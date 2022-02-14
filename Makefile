.PHONY: release
release:
	maike2 --configfiles=buildconfig/base.json,buildconfig/release.json --target-dir=__targets_rel

.PHONY: debug
debug:
	maike2 --configfiles=buildconfig/base.json --target-dir=__targets_dbg

.PHONY: clean
clean:
	rm -rf __targets*

.PHONY: format
format:
	find \( -name '*.hpp' -or -name '*.cpp' \) -and -not -path './__targets*' -and -not -path './fruit*' -and -not -path './pixel_store*' -and -not -path './vma*' -and -not -path './VulkanMemoryAllocator*' | xargs clang-format-12 -i
