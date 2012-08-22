#include <mozilla/ModuleUtils.h>
#include <nsIClassInfoImpl.h>

#include "FishjamHello_Impl.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsHelloFishjam)

// The following line defines a kNS_SAMPLE_CID CID variable.
NS_DEFINE_NAMED_CID(NS_HELLOFISHJAM_CID);

static const mozilla::Module::CIDEntry HelloFishjam_CIDs[] = {
	{ &kNS_HELLOFISHJAM_CID, false, NULL, nsHelloFishjamConstructor },
	{ NULL }
};

// Build a table of ClassIDs (CIDs) which are implemented by this module. CIDs
// should be completely unique UUIDs.
// each entry has the form { CID, service, factoryproc, constructorproc }
// where factoryproc is usually NULL.
static const mozilla::Module::ContractIDEntry HelloFishjam_Contracts[] = {
	{ NS_HELLOFISHJAM_CONTRACT_ID, &kNS_HELLOFISHJAM_CID },
	{ NULL }
};

// Category entries are category/key/value triples which can be used
// to register contract ID as content handlers or to observe certain
// notifications. Most modules do not need to register any category
// entries: this is just a sample of how you'd do it.
// @see nsICategoryManager for information on retrieving category data.
static const mozilla::Module::CategoryEntry HelloFishjam_Categories[] = {
	{ "my-category", "my-key", NS_HELLOFISHJAM_CONTRACT_ID },
	{ NULL }
};

static const mozilla::Module HelloFishjam_Module = {
	mozilla::Module::kVersion,
	HelloFishjam_CIDs,
	HelloFishjam_Contracts,
	HelloFishjam_Categories
};



// The following line implements the one-and-only "NSModule" symbol exported from this
// shared library.
NSMODULE_DEFN(nsSampleModule) = &HelloFishjam_Module;

// The following line implements the one-and-only "NSGetModule" symbol
// for compatibility with mozilla 1.9.2. You should only use this
// if you need a binary which is backwards-compatible and if you use
// interfaces carefully across multiple versions.
NS_IMPL_MOZILLA192_NSGETMODULE(&HelloFishjam_Module)

//static nsModuleComponentInfo components[] =
//{
//	{
//		NS_HELLOFISHJAM_CLASSNAME, 
//		NS_HELLOFISHJAM_CID,
//		NS_HELLOFISHJAM_CONTRACTID,
//		CnsHelloFishjamConstructor,
//	}
//};
//
//NS_IMPL_NSGETMODULE("nsHelloFishjamModule", components) 

