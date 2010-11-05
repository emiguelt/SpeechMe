TEMPLATE = app
TARGET = SpeechMe 

QT        += core \
    gui \
    network

HEADERS   += inc/SpeechMe.h \
    inc/Configuration.h \
    inc/speechpad.h \
    inc/speechremote.h
SOURCES   += src/SpeechMe_reg.rss \
    src/main.cpp \
    src/SpeechMe.cpp \
    src/Configuration.cpp \
    src/speechpad.cpp \
    src/speechremote.cpp
FORMS	  += ui/SpeechMe.ui \
    ui/config.ui \
    ui/speechpad.ui
RESOURCES +=

INCLUDEPATH += C:/sphinx/pocketsphinx/include \
    C:/sphinx/sphinxbase/include/s60 \
    C:/sphinx/sphinxbase/include/sphinxbase \
    C:/sphinx/sphinxbase/include \
    C:/sphinx/msrs/inc
RESOURCES += 
symbian{
TARGET.UID3 = 0xEE74B26E
TARGET.EPOCSTACKSIZE = 0x14000
TARGET.EPOCHEAPSIZE = 0x020000 0x800000 
TARGET.CAPABILITY = "UserEnvironment"
LIBS += -lsphinxbase -lpocketsphinx -lmsrs
myfiles.sources = sphinxbase.dll pocketsphinx.dll msrs.dll
myfiles.path = /sys/bin
DEPLOYMENT += myfiles
}
