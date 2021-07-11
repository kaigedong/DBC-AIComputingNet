#include "ResourceManager.h"
#include "comm.h"
#include "common/util.h"

using namespace matrix::core;

std::string ResourceManager::os_name() {
    std::string os_name = run_shell("cat /etc/os-release | grep VERSION | awk -F '=' '{print $2}'| tr -d '\"'");
    os_name = string_util::rtrim(os_name, '\n');
    std::string os_version = run_shell("uname -o -r");
    os_version = string_util::rtrim(os_version, '\n');
    return os_name + " " + os_version;
}

std::string ResourceManager::cpu_type() {
    std::string cpu_type = run_shell("cat /proc/cpuinfo |grep \"model name\" | awk -F \":\" '{print $2}'| uniq | awk -F \"@\" '{print $1}' |xargs");
    cpu_type = string_util::rtrim(cpu_type, '\n');
    return cpu_type;
}

int32_t ResourceManager::cpu_cores() {
    std::string cpu_cores = run_shell("cat /proc/cpuinfo |grep \"model name\" | awk -F \":\" '{print $2}'| wc -l");
    cpu_cores = string_util::rtrim(cpu_cores, '\n');
    return atoi(cpu_cores.c_str());
}

int32_t ResourceManager::mem_total() {
    std::string mem_total = run_shell("free  -g | grep \"Mem:\" | awk '{print $2}'");
    mem_total = string_util::rtrim(mem_total, '\n');
    return atoi(mem_total.c_str());
}

int32_t ResourceManager::mem_used() {
    std::string mem_used = run_shell("free  -g | grep \"Mem:\" | awk '{print $3}'");
    mem_used = string_util::rtrim(mem_used, '\n');
    return atoi(mem_used.c_str());
}

int32_t ResourceManager::mem_free() {
    std::string mem_free = run_shell("free  -g | grep \"Mem:\" | awk '{print $4}'");
    mem_free = string_util::rtrim(mem_free, '\n');
    return atoi(mem_free.c_str());
}

int32_t ResourceManager::mem_buff_cache() {
    std::string mem_buff_cache = run_shell("free  -g | grep \"Mem:\" | awk '{print $6}'");
    mem_buff_cache = string_util::rtrim(mem_buff_cache, '\n');
    return atoi(mem_buff_cache.c_str());
}

int32_t ResourceManager::disk_size() {
    std::string disk_size = run_shell("df -lh / | tail -1 | awk '{print $2}'");
    disk_size = string_util::rtrim(disk_size, '\n');
    return atoi(disk_size.c_str());
}

int32_t ResourceManager::disk_used() {
    std::string disk_used = run_shell("df -lh / | tail -1 | awk '{print $3}'");
    disk_used = string_util::rtrim(disk_used, '\n');
    return atoi(disk_used.c_str());
}

int32_t ResourceManager::disk_free() {
    std::string disk_free = run_shell("df -lh / | tail -1 | awk '{print $4}'");
    disk_free = string_util::rtrim(disk_free, '\n');
    return atoi(disk_free.c_str());
}

std::string ResourceManager::disk_type() {
    std::string disk = run_shell("df -lh / | tail -1 | awk '{print $1}' | awk -F\"/\" '{print $3}'");
    disk = string_util::rtrim(disk, '\n');
    std::string cmd = "lsblk -o name,rota | grep " + disk + " | awk '{if($2==\"1\")print \"HDD\"; else print \"SSD\"}'";
    std::string disk_type = run_shell(cmd.c_str());
    disk_type = string_util::rtrim(disk_type, '\n');
    return disk_type;
}
