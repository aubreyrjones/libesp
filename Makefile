all:
	python scons_local/scons.py -j 4 tests=true debug=true

notest:
	python scons_local/scons.py -j 4 tests=false

one:
	python scons_local/scons.py tests=true

clean:
	python scons_local/scons.py -c tests=true
	rm -f *.esp
	rm -f *.sqlite

release:
	python scons_local/scons.py -j 4 mode=release tests=true

deps:
	python scons_local/scons.py --dry-run --tree=prune tests=true

package:
	python scons_local/scons.py -f sconsPacker -C builds build_64=true
