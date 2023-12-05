import engine
import engine_draw
import engine_debug
import engine_input
import engine_physics
from engine_physics import PhysicsShapeRectangle
from engine_nodes import EmptyNode, Sprite2DNode, Rectangle2DNode, CameraNode, Physics2DNode
from engine_math import Vector3, Vector2, Rectangle
from engine_resources import TextureResource
import gc
import math
import os
print("dir(engine):",dir(engine))
print("dir(EmptyNode):", dir(EmptyNode))
print("dir(Sprite2DNode):", dir(Sprite2DNode))
print("dir(Rectangle2DNode):", dir(Rectangle2DNode))
print("dir(Vector3):", dir(Vector3))
print("dir(Rectangle):", dir(Rectangle))

engine_debug.debug_enable_all()
#engine_debug.debug_enable_setting(engine_debug.debug_setting_info)




class MyNodeCam(CameraNode):
    def __init__(self):
        super().__init__(self)

    def tick(self):
        if True:
            self.rotation.z += 0.1
        #if engine_input.is_bumper_right_pressed():
        #    self.rotation.z -= 0.1

        if engine_input.is_dpad_up_pressed():
            self.position.y -= 0.5
        if engine_input.is_dpad_down_pressed():
            self.position.y += 0.5

        if engine_input.is_dpad_left_pressed():
            self.position.x -= 0.5
        if engine_input.is_dpad_right_pressed():
            self.position.x += 0.5

c = MyNodeCam()


r0 = Rectangle2DNode()
r0.position.x = 128/2
r0.position.y = 128/2
r0.width = 25
r0.height = 10
r0.rotation = 45

r1 = Rectangle2DNode()
r1.position.x = 128/2
r1.position.y = 0
r1.width = 25
r1.height = 10

r2 = Rectangle2DNode()
r2.position.x = 128/2
r2.position.y = 127
r2.width = 25
r2.height = 10

n1 = Physics2DNode()
n2 = Physics2DNode()

n1.physics_shape = engine_physics.PhysicsShapeCircle()
n2.physics_shape = engine_physics.PhysicsShapeCircle()

#print(dir(n1))
m = n2.test(n1)
#m = engine_physics.PhysicsManifold()

#print(dir(m))
print(m.mtv_x)
print(m.mtv_y)
print(m.nrm_x)
print(m.nrm_y)
print(m.con_x)
print(m.con_y)

#engine.start()





# print(resource.data)


# # machine.freq(250 * 1000 * 1000)

# # engine_debug.debug_enable_all()
# engine_debug.debug_enable_setting(engine_debug.debug_setting_performance)


# class MyNodeCam(CameraNode):
#     def __init__(self):
#         super().__init__(self)
    
# cam2 = MyNodeCam()
# cam3 = MyNodeCam()

# cam2.viewport.x=0
# cam2.viewport.y=64
# cam2.viewport.width=64
# cam2.viewport.height=64

# cam3.viewport.x=64
# cam3.viewport.y=64
# cam3.viewport.width=64
# cam3.viewport.height=64



# b = Rectangle2DNode()
# b.height = 100
# b.position.x = b.position.x + 10.5
# b.position.y = b.position.y + 20.5
# b.color = 0b0000011101100001

# class MyRect2D(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)






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