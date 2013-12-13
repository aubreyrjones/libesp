all:
	python scons_local/scons.py -j 4

one:
	python scons_local/scons.py 

clean:
	python scons_local/scons.py -c

release:
	python scons_local/scons.py -j 4 mode=release

profile:
	python scons_local/scons.py -j 4 mode=profile

deps:
	python scons_local/scons.py --dry-run --tree=prune

package:
	python scons_local/scons.py -f sconsPacker -C builds build_64=true
