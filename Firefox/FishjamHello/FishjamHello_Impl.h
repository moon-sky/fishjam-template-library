#ifndef FISHJAM_HELLO_IMPL_H
#define FISHJAM_HELLO_IMPL_H

#include "FishjamHello.h"
#include "nsStringAPI.h"

#define NS_HELLOFISHJAM_CID   {0x7db47219, 0xfba5, 0x4e22, { 0xb5, 0x42, 0x40, 0x5e, 0x45, 0x44, 0x0c, 0x88 }}
#define NS_HELLOFISHJAM_CLASS_NAME	"Fishjam Helloworld"
#define NS_HELLOFISHJAM_CONTRACT_ID "@fishjam.com/hello;1"

class nsHelloFishjam : public nsIHelloFishjam
{
public:
	NS_DECL_ISUPPORTS
	NS_DECL_NSIHELLOFISHJAM

	nsHelloFishjam();

private:
	virtual ~nsHelloFishjam();

protected:
	/* additional members */
};

#endif //FISHJAM_HELLO_IMPL_H