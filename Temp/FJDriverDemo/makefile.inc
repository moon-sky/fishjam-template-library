#_LNG=$(LANGUAGE)
#_INX=.
#SIGNSYS=signtool sign /v /ac sign/VeriSignG5.cer /f sign/NHNCorp_20121130-20150130.pfx /p wood5566 /t http://timestamp.verisign.com/scripts/timestamp.dll $@ 
#STAMP=stampinf -f $@ -a $(_BUILDARCH) -k $(KMDF_VERSION_MAJOR).$(KMDF_VERSION_MINOR)


!IF "$(_BUILDARCH)" == "x86"
TARGETNAME=FJDriverDemo32
NO_SAFESEH=1
!ELSEIF "$(_BUILDARCH)" == "AMD64"
TARGETNAME=FJDriverDemo64
!ELSEIF "$(_BUILDARCH)" == "IA64"
TARGETNAME=FJDriverDemoIa64
!ENDIF



#$(OBJ_PATH)\$(O)\$(TARGETNAME).sys:
#    echo "need copy "
#   copy $(_INX)\$(@B).inx $@

