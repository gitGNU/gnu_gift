data_DATA= gift-ihandshake.mrml gift-iconfigure.mrml

MRMLCONFIG=sed "s:__DATADIR__:@datadir@:g" < $< > $@


EXTRA_DIST= gift-ihandshake.pre-mrml gift-iconfigure.pre-mrml	

gift-ihandshake.mrml:gift-ihandshake.pre-mrml
	$(MRMLCONFIG)

gift-iconfigure.mrml:gift-iconfigure.pre-mrml	
	$(MRMLCONFIG)
