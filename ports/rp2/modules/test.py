import engine
import engine_draw
import engine_debug
import engine_input
from engine_nodes import EmptyNode, Sprite2DNode, Rectangle2DNode, CameraNode
from engine_math import Vector3, Rectangle
import gc
import math
# import machine
print("dir(engine):",dir(engine))
print("dir(EmptyNode):", dir(EmptyNode))
print("dir(Sprite2DNode):", dir(Sprite2DNode))
print("dir(Rectangle2DNode):", dir(Rectangle2DNode))
print("dir(Vector3):", dir(Vector3))
print("dir(Rectangle):", dir(Rectangle))

# machine.freq(250 * 1000 * 1000)

# engine_debug.debug_enable_all()
engine_debug.debug_enable_setting(engine_debug.debug_setting_performance)


class MyNodeCam(CameraNode):
    def __init__(self):
        super().__init__(self)
    
    def tick(self):
        if engine_input.is_dpad_left_pressed():
            self.position.x = self.position.x - 1
        elif engine_input.is_dpad_right_pressed():
            self.position.x = self.position.x + 1
        
        if engine_input.is_dpad_up_pressed():
            self.position.y = self.position.y - 1
        elif engine_input.is_dpad_down_pressed():
            self.position.y = self.position.y + 1

cam0 = MyNodeCam()
cam1 = CameraNode()
cam2 = MyNodeCam()
cam3 = MyNodeCam()


cam0.viewport.x=0
cam0.viewport.y=0
cam0.viewport.width=64
cam0.viewport.height=64

cam1.viewport.x=64
cam1.viewport.y=0
cam1.viewport.width=64
cam1.viewport.height=64

cam2.viewport.x=0
cam2.viewport.y=64
cam2.viewport.width=64
cam2.viewport.height=64

cam3.viewport.x=64
cam3.viewport.y=64
cam3.viewport.width=64
cam3.viewport.height=64



b = Rectangle2DNode()
b.height = 100
b.position.x = b.position.x + 10.5
b.position.y = b.position.y + 20.5
b.color = 0b0000011101100001


class MyRect2D(Rectangle2DNode):
    def __init__(self):
        super().__init__(self)

        self.color = 0b1111100000011111

        self.angle = 0

        self.center_x = 64
        self.center_y = 64
    
    def tick(self):
        print("TEST")
        self.width = 50
        self.angle = self.angle + 0.01
        self.position.x = self.center_x  + (25 * math.cos(self.angle))
        self.position.y = self.center_y + (25 * math.sin(self.angle))

        if engine_input.is_bumper_left_pressed():
            self.center_x  = self.center_x - 1
        elif engine_input.is_bumper_right_pressed():
            self.center_x  = self.center_x + 1

a = MyRect2D()


engine.start()



# print(dir(a))
# print(a)


# for i in range(10):
#     test0 = Rectangle2DNode()
#     test0.position.x = test0.position.x + i*2
#     test0.position.y = test0.position.y + i*2
#     print(test0.width)

# class MyRect2D(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)

#         self.width = 5
#         self.color = 0b1111100000011111
#         self.position.x = 10
#         self.angle = 0
    
#     def tick(self):
#         self.angle = self.angle + 0.1
#         self.position.x = self.position.x + (5 * math.cos(self.angle))
#         self.position.y = self.position.y + (5 * math.sin(self.angle))


# test1 = MyRect2D()
# print(test1.width)



# class MyNodeC(EmptyNode):
#     def __init__(self):
#         super().__init__(self)
    
#     def tick(self):
#         pass

# if(True):
#     c = MyNodeC()
#     del c

#     d = MyRect2D()
#     del d

# gc.collect()


# # class MySprite(BitmapSpriteNode):
# #     def __init__(self):
# #         super().__init__(self)
# #         # self.height = 10

# #     def tick(self):
# #         pass
# #         # print("hi from sprite!")


# class MyRect2D(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)

#         # self.width = 5
#         # self.position.x = 10
    
#     def tick(self):
#         pass
#         # print("hi from sprite!")

# # a = MySprite()
# b = MyRect2D()
# c = MyNodeCam()
# b.set_width(5)
# # print(b.width)
# # b.width = 10
# # print(b.width)

# # a.position.x = 0
# # b.position.x = 10


# # print(dir(v))
# # print(v, end='')

# engine.start()


# class MyNodeA(EmptyNode):
#     def __init__(self):
#         super().__init__(self)

#     def tick(self):
#         pass
#         # print("hi from tick() A")


# class MyNodeB(EmptyNode):
#     def __init__(self):
#         super().__init__(self)


# class MyNodeC(EmptyNode):
#     def __init__(self):
#         super().__init__(self)


# class MyNodeD(EmptyNode):
#     def __init__(self):
#         super().__init__(self)
    
#     def tick(self):
#         pass
#         # print("hi from tick() D")



# class MySprite(BitmapSpriteNode):
#     def __init__(self):
#         super().__init__(self)

#     def tick(self):
#         pass
#         # print("hi from sprite!")


# class MyRect2D(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)
#         self.height = 10
    
#     def tick(self):
#         pass
#         # print("hi from sprite!")


# a = MyNodeA()
# b = MyNodeB()
# if(True):
#     c = MyNodeC()
#     del c
# d = MyNodeD()
# d.set_layer(7)

# e = MyNodeA()
# e1 = MyNodeA()
# e2 = MyNodeA()
# e3 = MyNodeA()
# e4 = MyNodeA()
# e5 = MyNodeA()
# e6 = MyNodeA()
# e7 = MyNodeA()

# print()


# v = Vector3()
# v.x = 2
# v.y = 2
# v.z = 2
# print(dir(v))
# print(v, end='')



# spr = MySprite()

# r = MyRect2D()


# gc.collect()

# engine.start()