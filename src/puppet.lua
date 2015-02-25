--
-- CS488 -- Introduction to Computer Graphics
-- 
-- puppet.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.sphere('torso')
rootnode:add_child(torso)
torso:set_material(white)
torso:scale(1.0, 2.0, 1.0);

shoulder = gr.sphere('shoulder')
torso:add_child(shoulder)
shoulder:set_material(white)
shoulder:translate(0.0, 1.0, 0.0);
shoulder:scale(1.5, 0.25, 0.75)


s_n_joint = gr.joint('s_n_joint', {-15.0, 0.0, 15.0}, {-15.0, 0.0, 15.0})
shoulder:add_child(s_n_joint)
s_n_joint:translate(0.0, 1.0, 0.0);

neck = gr.sphere('neck')
s_n_joint:add_child(neck)
neck:set_material(white)
neck:translate(0.0, 0.5, 0.0)
neck:scale(0.1, 1.0, 0.2)

n_h_joint = gr.joint('n_h_joint', {-15.0, 0.0, 15.0}, {-15.0, 0.0, 15.0})
neck:add_child(n_h_joint)
n_h_joint:translate(0.0, 1.0, 0.0);

head = gr.sphere('head')
n_h_joint:add_child(head)
head:set_material(white)
head:translate(0.0, 0.5, 0.0)
head:scale(3.33, 1.0, 3.33)

nose = gr.sphere('nose')
head:add_child(nose)
nose:set_material(red)
nose:translate(0.0, 0.0, 1.0)
nose:scale(0.2, 0.2, 1.0)

s_u_joint_r = gr.joint('s_u_joint_r', {-15.0, 0.0, 15.0}, {-15.0, 0.0, 15.0})
shoulder:add_child(s_u_joint_r)
s_u_joint_r:translate(1.0, 0.0, 0.0)
s_u_joint_r:rotate('x', 45.0)

upper_arm_r = gr.sphere('upper_arm_r')
s_u_joint_r:add_child(upper_arm_r)
upper_arm_r:set_material(white)
upper_arm_r:translate(0.0, 0.0, 3.0)
upper_arm_r:scale(0.5, 0.4, 0.2666)




-- s0 = gr.sphere('s0')
-- rootnode:add_child(s0)
-- s0:set_material(white)
-- 
-- s1 = gr.sphere('s1')
-- rootnode:add_child(s1)
-- s1:scale(0.1, 2.0, 0.1)
-- s1:set_material(red)
-- 
-- s2 = gr.sphere('s2')
-- rootnode:add_child(s2)
-- s2:translate(2.0, -2.0, 0.0)
-- s2:rotate('z', -90.0)
-- s2:scale(0.1, 2.0, 0.1)
-- s2:set_material(blue)
-- 
-- s3 = gr.sphere('s3')
-- rootnode:add_child(s3)
-- s3:scale(0.1, 0.1, 2.0)
-- s3:translate(0.0, -20.0, 1.0)
-- s3:set_material(green)
-- 
-- rootnode:translate(-0.75, 0.25, -10.0)
-- rootnode:rotate('y', -20.0)

return rootnode
