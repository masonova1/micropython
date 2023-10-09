print("# List of importable modules:")
exec("help('modules')")
print("")

import engine
print("# dir() of 'engine' after import:")
print(dir(engine))
print("")