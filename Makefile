PRO_FILE=gravastre.pro
BUILD_DIR=build/

all:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && qmake ../$(PRO_FILE)
