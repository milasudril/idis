.PHONY: release
release:
	maike2 --configfiles=buildconfig/base.json,buildconfig/release.json --target-dir=__targets_rel

.PHONY: debug
debug:
	maike2 --configfiles=buildconfig/base.json --target-dir=__targets_dbg

.PHONY: clean
clean:
	rm -rf __targets*