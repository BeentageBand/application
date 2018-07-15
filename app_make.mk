define $(_flavor_)_$(_feat_)_MAKE
$(_flavor_)_$(_feat_)_inc=app.h app_evs.h
$(_flavor_)_$(_feat_)_lib_objs=app app_fsm
$(_flavor_)_$(_feat_)_lib=app
endef

include $(PROJ_MAK_DIR)/epilog.mk