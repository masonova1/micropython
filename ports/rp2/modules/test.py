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




class MyNodeCam(CameraNode):
    def __init__(self):
        super().__init__(self)

    def tick(self):
        pass

c = MyNodeCam()


n1 = Physics2DNode()
n2 = Physics2DNode()

n1.physics_shape = engine_physics.PhysicsShapeCircle()
#n2.physics_shape = engine_physics.PhysicsShapeConvex([Vector2(3, 4), Vector2(5, 2), Vector2(2, 1)])
n2.physics_shape = engine_physics.PhysicsShapeRectangle()

n1.position = Vector2(7.4, 7.57)
n1.physics_shape.radius = 3

n2.position = Vector2(4, 0)

print("n1 data:")
print(n1.position)
print(n1.physics_shape.radius)

print("n2 data:")
print(n2.position)
print(n2.physics_shape.width)
print(n2.physics_shape.height)


class MyRect2D(Rectangle2DNode):
     def __init__(self):
         super().__init__(self)
    
     def tick(self):
         pass

b = MyRect2D()
c = MyRect2D()
c2 = MyRect2D()

def tickFunction():
    engine.tick()

print(n1.physics_shape.radius)
#print(n2.physics_shape.v_list)

#print(n2.physics_shape.n_list)

#print(n2.physics_shape.I)
#print(n2.physics_shape.area)
#print(n2.physics_shape.center)

m = n1.test(n2)

print(m.mtv_x)
print(m.mtv_y)
print(m.nrm_x)
print(m.nrm_y)
print(m.con_x)
print(m.con_y)

engine.init()

#engine.start()
#for i in range(3):
#    tickFunction()
#engine.tick()
#engine.tick()
#engine.tick()
tickFunction()
tickFunction()
tickFunction()