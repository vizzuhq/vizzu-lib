OS_TYPE = linux
PYTHON_BIN = python3
BIN_PATH = bin
ifeq ($(OS), Windows_NT)
	OS_TYPE = windows
	PYTHON_BIN = python
	BIN_PATH = Scripts
endif



.PHONY: clean \
	clean-dev-py update-dev-py-req install-dev-py-req check-dev-py \
	clean-dev-js check-dev-js \
	check format check-format lint check-lint check-typing

VIRTUAL_ENV = .venv_vizzu

DEV_PY_BUILD_FLAG = $(VIRTUAL_ENV)/DEV_PY_BUILD_FLAG
DEV_JS_BUILD_FLAG = node_modules/DEV_JS_BUILD_FLAG


clean: clean-dev-py clean-dev-js



# init

clean-dev-py:
	$(PYTHON_BIN) -c "import os, shutil;shutil.rmtree('$(VIRTUAL_ENV)') if os.path.exists('$(VIRTUAL_ENV)') else print('Nothing to be done for \'clean-dev\'')"

clean-dev-js:
	$(PYTHON_BIN) -c "import os, shutil;shutil.rmtree('node_modules') if os.path.exists('node_modules') else print('Nothing to be done for \'clean-dev-js\'')"

update-dev-py-req: $(DEV_PY_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/pip-compile --upgrade dev-requirements.in --resolver=backtracking

install-dev-py-req: $(DEV_PY_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/pip install -r dev-requirements.txt

check-dev-py:
	$(PYTHON_BIN) ./make/touch.py -f $(DEV_PY_BUILD_FLAG) --check

check-dev-js:
	$(PYTHON_BIN) ./make/touch.py -f $(DEV_JS_BUILD_FLAG) --check

dev-py: $(DEV_PY_BUILD_FLAG)

dev-js: $(DEV_JS_BUILD_FLAG)

$(DEV_PY_BUILD_FLAG):
	$(PYTHON_BIN) -m venv $(VIRTUAL_ENV)
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) -m pip install --upgrade pip
	$(VIRTUAL_ENV)/$(BIN_PATH)/pip install --use-pep517 -r dev-requirements.txt
	$(PYTHON_BIN) ./make/touch.py -f $(DEV_PY_BUILD_FLAG)

$(DEV_JS_BUILD_FLAG):
	npm install
	$(PYTHON_BIN) ./make/touch.py -f $(DEV_JS_BUILD_FLAG)



# ci

check: check-format check-lint check-typing

format: $(DEV_PY_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/black make mdformat
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./mdformat/mdformat.py $(VIRTUAL_ENV)/$(BIN_PATH)/mdformat \
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

check-format: $(DEV_PY_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/black --check make mdformat
	$(VIRTUAL_ENV)/$(BIN_PATH)/$(PYTHON_BIN) ./mdformat/mdformat.py $(VIRTUAL_ENV)/$(BIN_PATH)/mdformat \
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

lint: $(DEV_PY_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/pylint make/*.py mdformat/*.py
	npm run eslint

check-lint: $(DEV_PY_BUILD_FLAG) $(DEV_JS_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/pylint make/*.py mdformat/*.py
	npm run check-eslint

check-typing: $(DEV_PY_BUILD_FLAG)
	$(VIRTUAL_ENV)/$(BIN_PATH)/mypy make mdformat