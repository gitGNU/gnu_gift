MRMLCONFIG=sed "s:__PERL_LOCATION__:@PERL@:g;s:__BINDIR__:@bindir@:g;s:__DATADIR__:@datadir@:g;s:__HOME__:${HOME}:g" < $< > $@

data_DATA= gift-config.mrml 

gift-config.mrml:gift-config.pre-mrml
	$(MRMLCONFIG)

CLEANFILES= gift-config.mrml