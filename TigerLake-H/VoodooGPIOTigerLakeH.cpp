//
//  VoodooGPIOTigerLakeH.cpp
//  VoodooGPIO
//

#include "VoodooGPIOTigerLakeH.hpp"

OSDefineMetaClassAndStructors(VoodooGPIOTigerLakeH, VoodooGPIOIntel);

bool VoodooGPIOTigerLakeH::start(IOService *provider) {
    this->pins = tglh_pins;
    this->npins = ARRAY_SIZE(tglh_pins);
    this->groups = tglh_groups;
    this->ngroups = ARRAY_SIZE(tglh_groups);
    this->functions = tglh_functions;
    this->nfunctions = ARRAY_SIZE(tglh_functions);
    this->communities = tglh_communities;
    this->ncommunities = ARRAY_SIZE(tglh_communities);

    IOLog("%s::Loading GPIO Data for TigerLake-H\n", getName());

    return VoodooGPIO::start(provider);
}
