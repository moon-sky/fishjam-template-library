#_LNG=$(LANGUAGE)
#_INX=.
#SIGNSYS=signtool sign /v /ac sign/VeriSignG5.cer /f sign/NHNCorp_20121130-20150130.pfx /p wood5566 /t http://timestamp.verisign.com/scripts/timestamp.dll $@ 
#STAMP=stampinf -f $@ -a $(_BUILDARCH) -k $(KMDF_VERSION_MAJOR).$(KMDF_VERSION_MINOR)



#$(OBJ_PATH)\$(O)\nComicDrv64.sys:
#    $(SIGNSYS)
#   copy $(_INX)\$(@B).inx $@

