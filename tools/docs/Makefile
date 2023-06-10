OS_TYPE = linux
PYTHON_BIN = python3
BIN_PATH = bin
ifeq ($(OS), Windows_NT)
	OS_TYPE = windows
	PYTHON_BIN = python
	BIN_PATH = Scripts
endif



.PHONY: clean \
	clean-dev update-dev-req install-dev-req \
	check format check-format lint check-lint

VIRTUAL_ENV = .venv_vizzu

DEV_BUILD_FLAG = $(VIRTUAL_ENV)/DEV_BUILD_FLAG
DEV_JS_BUILD_FLAG = $(VIRTUAL_ENV)/DEV_JS_BUILD_FLAG



clean: clean-dev



# init

clean-dev:
	$(PYTHON_BIN) -c "import os, shutil;shutil.rmtree('$(VIRTUAL_ENV)') if os.path.exists('$(VIRTUAL_ENV)') else print('Nothing to be done for \'clean-dev\'')"

update-dev-req: $(DEV_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/pip-compile --upgrade dev-requirements.in

install-dev-req:
	$(VIRTUAL_ENV)/$(BIN_PATH)/pip install -r dev-requirements.txt

touch-dev:
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./touch.py -f $(DEV_BUILD_FLAG)

touch-dev-js:
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./touch.py -f $(DEV_JS_BUILD_FLAG)

dev: $(DEV_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)

$(DEV_BUILD_FLAG):
	$(PYTHON_BIN) -m venv $(VIRTUAL_ENV)
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) -m pip install --upgrade pip
	$(MAKE) -f Makefile install-dev-req
	$(MAKE) -f Makefile touch-dev

$(DEV_JS_BUILD_FLAG):
	npm install
	$(MAKE) -f Makefile touch-dev-js



# ci

check: check-format check-lint

format: $(DEV_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./mdformat.py $(VIRTUAL_ENV)/$(BIN_PATH)/mdformat \
		--wrap 80 \
		--end-of-line keep \
		--line-length 70 \
		../../docs \
		../../README.md \
		../../CODE_OF_CONDUCT.md \
		../../CONTRIBUTING.md \
		../../FAQ.md \
		../../PROJECTS.md
	npm run prettier

check-format: $(DEV_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./mdformat.py $(VIRTUAL_ENV)/$(BIN_PATH)/mdformat \
		--check \
		--wrap 80 \
		--end-of-line keep \
		--line-length 70 \
		../../docs \
		../../README.md \
		../../CODE_OF_CONDUCT.md \
		../../CONTRIBUTING.md \
		../../FAQ.md \
		../../PROJECTS.md
	npm run check-prettier

lint: $(DEV_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	npm run eslint

check-lint: $(DEV_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	npm run check-eslint
