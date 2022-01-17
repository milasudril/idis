.PHONY: release
release:
	maike2 --configfiles=buildconfig/base.json,buildconfig/release.json --target-dir=__targets_rel --source-dir=engine

.PHONY: debug
debug:
	maike2 --configfiles=buildconfig/base.json --target-dir=__targets_dbg --source-dir=engine

.PHONY: clean
clean:
	rm -rf __targets*

.PHONY: format
format:
	find \( -name '*.hpp' -or -name '*.cpp' \) -and -not -path './__targets*' | xargs clang-format-12 -i
