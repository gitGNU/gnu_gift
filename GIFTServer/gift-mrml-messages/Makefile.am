data_DATA= gift-ihandshake.mrml gift-iconfigure.mrml

MRMLCONFIG=sed "s:__DATADIR__:@datadir@:g" < $< > $@


gift-ihandshake.mrml:gift-ihandshake.pre-mrml
	$(MRMLCONFIG)

gift-iconfigure.mrml:gift-iconfigure.pre-mrml	
	$(MRMLCONFIG)
