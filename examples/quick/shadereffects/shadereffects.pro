TEMPLATE = app

QT += quick qml
SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/quick/shadereffects
qml.files = shadereffects.qml content
qml.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/quick/shadereffects
INSTALLS += target qml

