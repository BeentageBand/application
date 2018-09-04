define $(_flavor_)_$(_feat_)_MAKE
$(_flavor_)_$(_feat_)_inc=application.h application_mailist.h application_types.h
$(_flavor_)_$(_feat_)_lib_objs=application application_fsm
$(_flavor_)_$(_feat_)_lib=application
endef

include $(PROJ_MAK_DIR)/epilog.mk