TEMPLATE = app
TARGET = SpeechMe
QT += core \
    gui \
    network \
    webkit
HEADERS += inc/progressdialog.h \
    inc/SpeechMe.h \
    inc/ConfigUi.h \
    inc/Configuration.h \
    inc/speechpad.h \
    inc/speechremote.h \
    inc/RemoteClient.h \
    inc/speechweb.h \
    inc/decoderthread.h
SOURCES += src/progressdialog.cpp \
    src/SpeechMe_reg.rss \
    src/main.cpp \
    src/SpeechMe.cpp \
    src/ConfigUi.cpp \
    src/Configuration.cpp \
    src/speechpad.cpp \
    src/speechremote.cpp \
    src/RemoteClient.cpp \
    src/speechweb.cpp \
    src/decoderthread.cpp
FORMS += ui/progressdialog.ui \
    ui/config.ui \
    ui/speechpad.ui \
    ui/speechweb.ui \
    ui/SpeechMe2.ui
RESOURCES += 
INCLUDEPATH += C:/sphinx/pocketsphinx/include \
    C:/sphinx/sphinxbase/include/s60 \
    C:/sphinx/sphinxbase/include/sphinxbase \
    C:/sphinx/sphinxbase/include \
    C:/sphinx/msrs/inc
RESOURCES += 
symbian { 
    TARGET.UID3 = 0xEE74B26E
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 \
        0x800000
    TARGET.CAPABILITY = "NetworkServices UserEnvironment"
    LIBS += -lsphinxbase \
        -lpocketsphinx \
        -lmsrs
    myfiles.sources = sphinxbase.dll \
        pocketsphinx.dll \
        msrs.dll
    myfiles.path = /sys/bin
    DEPLOYMENT += myfiles
}
