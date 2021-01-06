SRC=./src/
INC=./include/
OUT=./target/
.DEFAULT_GOAL := help

help: ## displays this message!
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: ## builds the project!
	@echo "Building project..."
	$(MAKE) main

clean: ## cleans up the intermediate files
	@echo "Cleaning up..."
	rm -r ./target/*.o

main: $(SRC)main.cpp
	@echo "Building..............................main.exe"
	@g++ -o $(OUT)main.exe $(SRC)main.cpp 

