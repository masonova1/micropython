import engine
from engine import EmptyNode, CameraNode, BitmapSpriteNode, Vector3, Rectangle
import gc
print("dir(engine):",dir(engine))
print("dir(EmptyNode):", dir(EmptyNode))
print("dir(CameraNode):", dir(CameraNode))
print("dir(BitmapSpriteNode):", dir(BitmapSpriteNode))
print("dir(Vector3):", dir(Vector3))
print("dir(Rectangle):", dir(Rectangle))

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

class MySprite(BitmapSpriteNode):
    def __init__(self):
        super().__init__(self)
    
    def tick(self):
        # pass
        print("hi from sprite!")


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


cam0 = MyNodeCam()
cam1 = MyNodeCam()


spr = MySprite()


gc.collect()

engine.start()