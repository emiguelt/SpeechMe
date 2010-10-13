TEMPLATE = app
TARGET = QtSpeech
QT += core \
    gui
HEADERS += DecoderControl.h \
    QtSpeech.loc \
    QtSpeech.h
SOURCES += DecoderControl.cpp \
    QtSpeech.rss \
    QtSpeech_reg.rss \
    main.cpp \
    QtSpeech.cpp
FORMS += QtSpeech.ui
INCLUDEPATH += C:\\sphinx\pocketsphinx\include \
    C:\sphinx\sphinxbase\include\s60 \
    C:\sphinx\sphinxbase\include\sphinxbase \
    C:\sphinx\sphinxbase\include
RESOURCES += 
symbian{
TARGET.UID3 = 0xEB71B053
TARGET.EPOCSTACKSIZE = 0x14000
TARGET.EPOCHEAPSIZE = 0x020000 0x800000
LIBS += -lsphinxbase -lpocketsphinx
myfiles.sources = sphinxbase.dll pocketsphinx.dll
myfiles.path = /sys/bin
DEPLOYMENT += myfiles
}

LIBS += -lsphinxbase -lpocketsphinx