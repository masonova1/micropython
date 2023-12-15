import engine_debug
engine_debug.debug_enable_all()
# engine_debug.debug_enable_setting(engine_debug.debug_setting_errors)
# engine_debug.debug_enable_setting(engine_debug.debug_setting_warnings)
# engine_debug.debug_enable_setting(engine_debug.debug_setting_performance)

import engine
import engine_draw
import engine_input
import engine_physics
from engine_physics import PhysicsShapeRectangle
from engine_nodes import EmptyNode, Sprite2DNode, Rectangle2DNode, Circle2DNode, CameraNode, Physics2DNode
from engine_math import Vector3, Vector2, Rectangle
from engine_resources import TextureResource, FontResource
import gc
import math
import os
import time


texture = TextureResource("128x128.bin", 128, 128)
sprite = Sprite2DNode(texture)
camera = CameraNode()

# nodes = []
# for i in range(25):
#     nodes.append(Sprite2DNode(texture))

# engine.start()
engine.init()
# engine.tick()
# engine.tick()

for i in range(100):
    engine.tick()


engine.stop()


# print("dir(engine):", dir(engine))
# print("dir(EmptyNode):", dir(EmptyNode))
# print("dir(Sprite2DNode):", dir(Sprite2DNode))
# print("dir(Rectangle2DNode):", dir(Rectangle2DNode))
# print("dir(Vector3):", dir(Vector3))
# print("dir(Rectangle):", dir(Rectangle))

# print(os.listdir())

# class MyNodeCam(CameraNode):
#     def __init__(self):
#         super().__init__(self)

#     def tick(self):
#         pass


# circle0 = Circle2DNode()
# circle0.position.x = 0
# circle0.position.y = -30
# circle0.radius = 5
# circle0.color = 0b0000011111100000



# class MyRect(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)
    
#     def tick(self):
#         global circle0

#         self.rotation -= 0.35
#         circle0.rotation -= 0.35

# class MyRect2(Rectangle2DNode):
#     def __init__(self):
#         super().__init__(self)
    
#     def tick(self):
#         pass

# c = MyNodeCam()


# r0 = MyRect()
# r0.position.x = 128/2
# r0.position.y = 128/2
# r0.width = 25
# r0.height = 10
# r0.rotation = 0
# r0.color = 0b1111100000011111


# r1 = Rectangle2DNode()
# r1.position.x = 0
# r1.position.y = 30
# r1.width = 25
# r1.height = 10
# r1.color = 0b1111100000000000
# r0.add_child(r1)

# r2 = Rectangle2DNode()
# r2.position.x = 0
# r2.position.y = 30
# r2.width = 25
# r2.height = 10
# r2.color = 0b0000000000011111
# r1.add_child(r2)


# r0.add_child(circle0)

# circle1 = Circle2DNode()
# circle1.position.x = 0
# circle1.position.y = -30
# circle1.radius = 8
# circle1.color = 0b1111100000000000
# circle0.add_child(circle1)


# r3 = MyRect2()
# r3.position.x = -30
# r3.position.y = 0
# r3.width = 25
# r3.height = 10
# r3.color = 0b0101010101011111
# r0.add_child(r3)

# engine.start()

