import engine
from engine import EmptyNode, CameraNode, Vector3
import gc
print("dir(engine):",dir(engine))
print("dir(EmptyNode):", dir(EmptyNode))
print("dir(CameraNode):", dir(CameraNode))
print("dir(Vector3):", dir(Vector3))

engine.set_debug_print_level(engine.debug_print_level_all)

class MyNodeA(EmptyNode):
    def __init__(self):
        super().__init__(self)

    def tick(self):
        print("hi from tick() A")


class MyNodeB(EmptyNode):
    def __init__(self):
        super().__init__(self)


class MyNodeC(EmptyNode):
    def __init__(self):
        super().__init__(self)


class MyNodeD(EmptyNode):
    def __init__(self):
        super().__init__(self)
    
    def tick(self):
        # pass
        print("hi from tick() D")

class MyNodeCam(CameraNode):
    def __init__(self):
        super().__init__(self)
    
    def tick(self):
        # pass
        print("hi from camera!")


a = MyNodeA()
b = MyNodeB()
if(True):
    c = MyNodeC()
    del c
d = MyNodeD()
d.set_layer(7)


print()


v = Vector3()
v.x = 2
v.y = 2
v.z = 2
print(dir(v))
print(v, end='')


cam = MyNodeCam()
print(dir(cam))
print(dir(cam.position))
print(dir(cam.viewport))
print(cam.viewport, end='')
print(cam.position, end='')
cam.position.x = 1
cam.position.y = 1
cam.position.z = 1
print(cam.position, end='')


gc.collect()

# engine.start()