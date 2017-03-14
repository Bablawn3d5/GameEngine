import sys
import imp

'''
Find modules by hierarchical module names.

imp.find_module() does not handle hierarchical module names (names containing dots).


Example: find_dotted_module('mypackage.myfile')

Use case: Test discovery without loading (otherwise coverage does not see the lines which are executed 
          at import time)
'''
def find_dotted_module(name, path=None):
    for x in name.split('.'):
        if path is not None:
            path=[path]
        file, path, descr = imp.find_module(x, path)
    return file, path, descr

def reload(name, skip=False):
    # Inefficently load modules.
    # Call default import.
    __import__(name)
    try:
        # TODO(SMA): This isn't hooked up on C++ end. Possibily passthough here?
        if skip:
            return sys.modules[name]
        # Nuke it if its imported
        del sys.modules[name]
    except KeyError:
        pass

    # If any of the following calls raises an exception,
    # there's a problem we can't handle -- let the caller handle it.
    fp, pathname, description = find_dotted_module(name)

    try:
        # load_module will reload the module here.
        module = imp.load_module(name, fp, pathname, description)
        return module
    finally:
        # Since we may exit via an exception, close fp explicitly.
        if fp:
            fp.close()