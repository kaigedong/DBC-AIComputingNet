#include "virImpl.h"
#include <boost/filesystem.hpp>
#include "tinyxml2.h"

#define __DEBUG__ 1

#ifdef __DEBUG__
#define DebugPrintf(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DebugPrintf(format, ...)
#endif

// enum virDomainState
static const char* arrayDomainState[] = {"no state", "running", "blocked", "paused", "shutdown", "shutoff", "crashed", "pmsuspended", "last"};
// enum virDomainEventType
static const char* arrayEventType[] = {"VIR_DOMAIN_EVENT_DEFINED", "VIR_DOMAIN_EVENT_UNDEFINED", "VIR_DOMAIN_EVENT_STARTED",
  "VIR_DOMAIN_EVENT_SUSPENDED", "VIR_DOMAIN_EVENT_RESUMED", "VIR_DOMAIN_EVENT_STOPPED",
  "VIR_DOMAIN_EVENT_SHUTDOWN", "VIR_DOMAIN_EVENT_PMSUSPENDED", "VIR_DOMAIN_EVENT_CRASHED"};
// enum event agent state
static const char* arrayEventAgentState[] = {"no state", "agent connected", "agent disconnected", "last"};
// enum event agent lifecycle reason
static const char* arrayEventAgentReason[] = {"unknown state change reason", "state changed due to domain start", "channel state changed", "last"};

int domain_event_lifecycle_cb(virConnectPtr conn, virDomainPtr dom, int event, int detail, void *opaque) {
    DebugPrintf("event lifecycle cb called, event=%d, detail=%d\n", event, detail);
    if (event >= 0 && event <= virDomainEventType::VIR_DOMAIN_EVENT_CRASHED) {
        const char* name = virDomainGetName(dom);
        int domain_state = 0;
        if (virDomainGetState(dom, &domain_state, NULL, 0) < 0) {
            domain_state = 0;
        }
        DebugPrintf("domain %s %s, state %s\n", name, arrayEventType[event], arrayDomainState[domain_state]);
    } else {
        DebugPrintf("unknowned event\n");
    }
}

void domain_event_agent_cb(virConnectPtr conn, virDomainPtr dom, int state, int reason, void *opaque) {
    DebugPrintf("event agent lifecycle cb called, state=%d, reason=%d\n", state, reason);
    if (state >= 0 && state <= virConnectDomainEventAgentLifecycleState::VIR_CONNECT_DOMAIN_EVENT_AGENT_LIFECYCLE_STATE_DISCONNECTED) {
        const char* name = virDomainGetName(dom);
        int domain_state = 0;
        if (virDomainGetState(dom, &domain_state, NULL, 0) < 0) {
            domain_state = 0;
        }
        DebugPrintf("event agent state: %s, reason: %s, domain state: %s\n", arrayEventAgentState[state],
            arrayEventAgentReason[reason], arrayDomainState[domain_state]);
    } else {
        DebugPrintf("unknowned event agent state\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////

void virConnectDeleter(virConnectPtr conn) {
    virConnectClose(conn);
    DebugPrintf("vir connect close\n");
}

void virDomainDeleter(virDomainPtr domain) {
    virDomainFree(domain);
    DebugPrintf("vir domain free\n");
}

void virErrorDeleter(virErrorPtr error) {
    virFreeError(error);
    DebugPrintf("vir error free\n");
}

/////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<virError> getLastError() {
    return std::shared_ptr<virError>(virGetLastError(), virErrorDeleter);
}

/////////////////////////////////////////////////////////////////////////////////

virDomainImpl::virDomainImpl(virDomainPtr domain)
    : domain_(std::shared_ptr<virDomain>(domain, virDomainDeleter)) {
}

virDomainImpl::~virDomainImpl() {

}

int32_t virDomainImpl::startDomain() {
    if (!domain_) return -1;
    return virDomainCreate(domain_.get());
}

int32_t virDomainImpl::suspendDomain() {
    if (!domain_) return -1;
    return virDomainSuspend(domain_.get());
}

int32_t virDomainImpl::resumeDomain() {
    if (!domain_) return -1;
    return virDomainResume(domain_.get());
}

int32_t virDomainImpl::rebootDomain(int flag) {
    if (!domain_) return -1;
    return virDomainReboot(domain_.get(), flag);
}

int32_t virDomainImpl::shutdownDomain() {
    if (!domain_) return -1;
    return virDomainShutdown(domain_.get());
}

int32_t virDomainImpl::destroyDomain() {
    if (!domain_) return -1;
    return virDomainDestroy(domain_.get());
}

int32_t virDomainImpl::resetDomain() {
    if (!domain_) return -1;
    return virDomainReset(domain_.get(), 0);
}

int32_t virDomainImpl::undefineDomain() {
    if (!domain_) return -1;
    return virDomainUndefine(domain_.get());
}

int32_t virDomainImpl::deleteDomain() {
    if (!domain_) return -1;
    virDomainInfo info;
    int ret = virDomainGetInfo(domain_.get(), &info);
    if (ret < 0) return ret;
    std::vector<std::string> disks;
    getDomainDisks(disks);
    if (info.state == VIR_DOMAIN_RUNNING) {
        ret = destroyDomain();
        if (ret < 0) return ret;
    }
    ret = undefineDomain();
    if (ret < 0) return ret;
    for (const auto& disk_file : disks) {
        boost::system::error_code error_code;
        if (boost::filesystem::exists(disk_file, error_code) && !error_code) {
            remove(disk_file.c_str());
            DebugPrintf("delete file: %s\n", disk_file.c_str());
        }
    }
    return 0;
}

int32_t virDomainImpl::isDomainActive() {
    if (!domain_) return -1;
    return virDomainIsActive(domain_.get());
}

int32_t virDomainImpl::getDomainDisks(std::vector<std::string> &disks) {
    if (!domain_) return -1;
    char* pContent = virDomainGetXMLDesc(domain_.get(), VIR_DOMAIN_XML_SECURE);
    if (!pContent) return -1;
    do {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError err = doc.Parse(pContent);
        if (err != tinyxml2::XML_SUCCESS) break;
        tinyxml2::XMLElement* root = doc.RootElement();
        tinyxml2::XMLElement* devices_node = root->FirstChildElement("devices");
        if (!devices_node) break;
        tinyxml2::XMLElement* disk_node = devices_node->FirstChildElement("disk");
        while (disk_node) {
            tinyxml2::XMLElement* disk_source_node = disk_node->FirstChildElement("source");
            std::string disk_file = disk_source_node->Attribute("file");
            disks.push_back(disk_file);
            disk_node = disk_node->NextSiblingElement("disk");
        }
    } while(0);

    free(pContent);
    return 0;
}

int32_t virDomainImpl::getDomainInterfaceAddress(std::vector<test::virDomainInterface> &difaces) {
    if (!domain_) return -1;
    virDomainInterfacePtr *ifaces = nullptr;
    int ifaces_count = 0;
    size_t i, j;
    test::virDomainInterface diface;

    if ((ifaces_count = virDomainInterfaceAddresses(domain_.get(), &ifaces,
            VIR_DOMAIN_INTERFACE_ADDRESSES_SRC_LEASE, 0)) < 0)
        goto cleanup;

    for (i = 0; i < ifaces_count; i++) {
        // DebugPrintf("name: %s", ifaces[i]->name);
        diface.name = ifaces[i]->name;
        if (ifaces[i]->hwaddr) {
            // DebugPrintf(" hwaddr: %s", ifaces[i]->hwaddr);
            diface.hwaddr = ifaces[i]->hwaddr;
        }

        for (j = 0; j < ifaces[i]->naddrs; j++) {
            virDomainIPAddressPtr ip_addr = ifaces[i]->addrs + j;
            // DebugPrintf("[addr: %s prefix: %d type: %d]",
            //     ip_addr->addr, ip_addr->prefix, ip_addr->type);
            diface.addrs.push_back(test::virDomainIPAddress{ip_addr->type, ip_addr->addr, ip_addr->prefix});
        }
        // DebugPrintf("\n");
        difaces.push_back(diface);
    }

cleanup:
    if (ifaces && ifaces_count > 0) {
        for (i = 0; i < ifaces_count; i++) {
            virDomainInterfaceFree(ifaces[i]);
        }
    }
    if (ifaces)
        free(ifaces);
    return ifaces_count;
}

int32_t virDomainImpl::setDomainUserPassword(const char *user, const char *password) {
    if (!domain_) return -1;
    return virDomainSetUserPassword(domain_.get(), user, password, 0);
}

int32_t virDomainImpl::getDomainInfo(virDomainInfoPtr info) {
    if (!domain_) return -1;
    return virDomainGetInfo(domain_.get(), info);
}

int32_t virDomainImpl::getDomainCPUStats(virTypedParameterPtr params, unsigned int nparams, int start_cpu, unsigned int ncpus, unsigned int flags) {
    if (!domain_) return -1;
    return virDomainGetCPUStats(domain_.get(), params, nparams, start_cpu, ncpus, flags);
}

int32_t virDomainImpl::getDomainMemoryStats(virDomainMemoryStatPtr stats, unsigned int nr_stats, unsigned int flags) {
    if (!domain_) return -1;
    return virDomainMemoryStats(domain_.get(), stats, nr_stats, flags);
}

int32_t virDomainImpl::getDomainBlockInfo(const char *disk, virDomainBlockInfoPtr info, unsigned int flags) {
    if (!domain_) return -1;
    return virDomainGetBlockInfo(domain_.get(), disk, info, flags);
}

int32_t virDomainImpl::getDomainBlockStats(const char *disk, virDomainBlockStatsPtr stats, size_t size) {
    if (!domain_) return -1;
    return virDomainBlockStats(domain_.get(), disk, stats, size);
}

int32_t virDomainImpl::getDomainNetworkStats(const char *device, virDomainInterfaceStatsPtr stats, size_t size) {
    if (!domain_) return -1;
    return virDomainInterfaceStats(domain_.get(), device, stats, size);
}

/////////////////////////////////////////////////////////////////////////////////

virTool::virTool()
    : conn_(nullptr)
    , dom_event_lifecycle_callback_id_(-1)
    , dom_event_agent_callback_id_(-1)
    , thread_quit_(1)
    , thread_event_loop_(nullptr) {
    int ret = virEventRegisterDefaultImpl();
    if (ret < 0) {
        DebugPrintf("virEventRegisterDefaultImpl failed\n");
    }
}

virTool::~virTool() {
    thread_quit_ = 1;
    if (thread_event_loop_) {
        if (thread_event_loop_->joinable())
            thread_event_loop_->join();
        delete thread_event_loop_;
    }
    if (conn_) {
        virConnectDomainEventDeregisterAny(conn_.get(), dom_event_lifecycle_callback_id_);
        virConnectDomainEventDeregisterAny(conn_.get(), dom_event_agent_callback_id_);
    }
}

bool virTool::openConnect(const char *name) {
    virConnectPtr connectPtr = virConnectOpen(name);
    if (connectPtr == nullptr) {
        return false;
    }
    conn_ = std::shared_ptr<virConnect>(connectPtr, virConnectDeleter);
    if (connectPtr) {
        dom_event_lifecycle_callback_id_ = virConnectDomainEventRegisterAny(connectPtr, NULL,
            virDomainEventID::VIR_DOMAIN_EVENT_ID_LIFECYCLE, VIR_DOMAIN_EVENT_CALLBACK(domain_event_lifecycle_cb), NULL, NULL);
        dom_event_agent_callback_id_ = virConnectDomainEventRegisterAny(connectPtr, NULL,
            virDomainEventID::VIR_DOMAIN_EVENT_ID_AGENT_LIFECYCLE, VIR_DOMAIN_EVENT_CALLBACK(domain_event_agent_cb), NULL, NULL);
        thread_quit_ = 0;
        thread_event_loop_ = new std::thread(&virTool::DefaultThreadFunc, this);
    }
    return !!conn_;
}

std::shared_ptr<virDomainImpl> virTool::defineDomain(const char *xml_content) {
    if (!conn_) return nullptr;
    virDomainPtr domainPtr = virDomainDefineXML(conn_.get(), xml_content);
    if (nullptr == domainPtr) {
        return nullptr;
    }
    return std::make_shared<virDomainImpl>(domainPtr);
}

std::shared_ptr<virDomainImpl> virTool::openDomain(const char *domain_name) {
    if (!conn_) return nullptr;
    virDomainPtr domainPtr = virDomainLookupByName(conn_.get(), domain_name);
    if (nullptr == domainPtr) {
        return nullptr;
    }
    return std::make_shared<virDomainImpl>(domainPtr);
}

void virTool::DefaultThreadFunc() {
  DebugPrintf("vir event loop thread begin\n");
  while (thread_quit_ == 0) {
    if (virEventRunDefaultImpl() < 0) {
        std::shared_ptr<virError> err = getLastError();
        DebugPrintf("virEventRunDefaultImpl failed: %s\n", err ? err->message : "");
    }
  }
  DebugPrintf("vir event loop thread end\n");
}

