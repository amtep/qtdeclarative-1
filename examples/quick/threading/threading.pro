TEMPLATE = app

QT += quick qml
SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/quick/threading
qml.files = threading.qml threadedlistmodel workerscript
qml.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/quick/threading
INSTALLS += target qml
