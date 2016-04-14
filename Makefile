PRO_FILE=gravastre.pro
BUILD_DIR=build/
DATA_DIR=data/


all:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && qmake ../$(PRO_FILE)


solarsystem:
	$(BUILD_DIR)gravastre $(DATA_DIR)solarsystem.json
