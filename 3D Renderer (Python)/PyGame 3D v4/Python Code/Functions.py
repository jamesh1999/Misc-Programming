import cProfile,Py3D,py_compile,sys

try:
    if sys.argv[1]=="p":
        cProfile.run("Py3D.main()")
    elif sys.argv[1]=="c":
        py_compile.compile("Py3D.py")
except IndexError:
    print("This file is for development purposes and is designed to be called by run.bat")
    input("Press enter to exit")
