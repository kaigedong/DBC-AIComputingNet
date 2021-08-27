#include "net_type_params.h"
#include "peer_seeds.h"

static peer_seeds g_main_peer_seeds[] =
{
    //{"1.2.3.4", 12345}
    {"111.44.254.182", 11118}
};

static const char* g_main_dns_seeds[] =
{
    "a.b.c"
};

void net_type_params::init_seeds()
{
    peer_seeds * hard_code_seeds = g_main_peer_seeds;
    int hard_code_seeds_count = sizeof(g_main_peer_seeds) / sizeof(peer_seeds);
    for (int i = 0; i < hard_code_seeds_count; i++)
    {
        m_hard_code_seeds.push_back(hard_code_seeds[i]);
    }

    const char **dns_seeds = g_main_dns_seeds;
    int dns_seeds_count = sizeof(g_main_dns_seeds) / sizeof(const char *);
    for (int i = 0; i < dns_seeds_count; i++)
    {
        m_dns_seeds.push_back(dns_seeds[i]);
    }
}