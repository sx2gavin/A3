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
shoulder:translate(0.0, 2.0, 0.0);
shoulder:scale(1.5, 0.5, 0.7)

s_n_joint = gr.joint('s_n_joint', {-15.0, 0.0, 15.0}, {-15.0, 0.0, 15.0})
shoulder:add_child(s_n_joint)
s_n_joint:translate(0.0, 0.5, 0.0);

neck = gr.sphere('neck')
s_n_joint:add_child(neck)
neck:set_material(white)
neck:translate(0.0, 0.1, 0.0)
neck:scale(0.2, 0.5, 0.2)

n_h_joint = gr.joint('n_h_joint', {-15.0, 0.0, 15.0}, {-15.0, 0.0, 15.0})
neck:add_child(n_h_joint)
n_h_joint:translate(0.0, 0.5, 0.0);

head = gr.sphere('head')
n_h_joint:add_child(head)
head:set_material(white)
head:translate(0.0, 0.1, 0.0)
head:scale(0.5, 0.5, 0.5)

nose = gr.sphere('nose')
head:add_child(nose)
nose:set_material(red)
nose:translate(0.0, 0.0, 0.25)
nose:scale(0.1, 0.1, 0.5)

-- right arm
s_u_joint_r = gr.joint('s_u_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
shoulder:add_child(s_u_joint_r)
s_u_joint_r:translate(1.5, 0.0, 0.0)
s_u_joint_r:rotate('z', -60.0)

upper_arm_r = gr.sphere('upper_arm_r')
s_u_joint_r:add_child(upper_arm_r)
upper_arm_r:set_material(blue)
upper_arm_r:translate(1.0, 0.0, 0.0)
upper_arm_r:scale(1.0, 0.2, 0.2)

u_f_joint_r = gr.joint('u_f_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
upper_arm_r:add_child(u_f_joint_r)
u_f_joint_r:translate(1.0, 0.0, 0.0)
u_f_joint_r:rotate('z', -30.0)

forearm_r = gr.sphere('forearm_r')
u_f_joint_r:add_child(forearm_r)
forearm_r:set_material(blue)
forearm_r:translate(0.9, 0.0, 0.0)
forearm_r:scale(1.0, 0.15, 0.15)

f_h_joint_r = gr.joint('f_h_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
forearm_r:add_child(f_h_joint_r)
f_h_joint_r:translate(1.0, 0.0, 0.0)

hand_r = gr.sphere('hand_r')
f_h_joint_r:add_child(hand_r)
hand_r:set_material(red)
hand_r:translate(0.2, 0.0, 0.0)
hand_r:scale(0.25, 0.25, 0.25)

-- left arm
s_u_joint_l = gr.joint('s_u_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
shoulder:add_child(s_u_joint_l)
s_u_joint_l:translate(-1.5, 0.0, 0.0)
s_u_joint_l:rotate('z', 60.0)

upper_arm_l = gr.sphere('upper_arm_l')
s_u_joint_l:add_child(upper_arm_l)
upper_arm_l:set_material(blue)
upper_arm_l:translate(-1.0, 0.0, 0.0)
upper_arm_l:scale(1.0, 0.2, 0.2)

u_f_joint_l = gr.joint('u_f_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
upper_arm_l:add_child(u_f_joint_l)
u_f_joint_l:translate(-1.0, 0.0, 0.0)
u_f_joint_l:rotate('z', 30.0)

forearm_l = gr.sphere('forearm_l')
u_f_joint_l:add_child(forearm_l)
forearm_l:set_material(blue)
forearm_l:translate(-0.9, 0.0, 0.0)
forearm_l:scale(1.0, 0.15, 0.15)

f_h_joint_l = gr.joint('f_h_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
forearm_l:add_child(f_h_joint_l)
f_h_joint_l:translate(-1.0, 0.0, 0.0)

hand_l = gr.sphere('hand_l')
f_h_joint_l:add_child(hand_l)
hand_l:set_material(red)
hand_l:translate(-0.2, 0.0, 0.0)
hand_l:scale(0.25, 0.25, 0.25)

hip = gr.sphere('hip')
torso:add_child(hip)
hip:set_material(white)
hip:translate(0.0, -2.0, 0.0)
hip:scale(1.0, 0.5, 0.7)


-- Right leg
h_t_joint_r = gr.joint('h_t_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
hip:add_child(h_t_joint_r)
h_t_joint_r:translate(0.5, 0.0, 0.0)

thigh_r = gr.sphere('thigh_r')
h_t_joint_r:add_child(thigh_r)
thigh_r:set_material(blue)
thigh_r:translate(0.0, -1.0, 0.0)
thigh_r:scale(0.3, 1.0, 0.3)

t_c_joint_r = gr.joint('t_c_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
thigh_r:add_child(t_c_joint_r)
t_c_joint_r:translate(0.0, -1.0, 0.0)

calf_r = gr.sphere('calf_r')
t_c_joint_r:add_child(calf_r)
calf_r:set_material(blue)
calf_r:translate(0.0, -0.9, 0.0)
calf_r:scale(0.2, 1.0, 0.2)

c_f_joint_r = gr.joint('c_f_joint_r', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
calf_r:add_child(c_f_joint_r)
c_f_joint_r:translate(0.0, -1.0, 0.0)

foot_r = gr.sphere('foot_r')
c_f_joint_r:add_child(foot_r)
foot_r:set_material(green)
foot_r:translate(0.0, 0.0, 0.5)
foot_r:scale(0.2, 0.2, 0.5)

-- left leg
h_t_joint_l = gr.joint('h_t_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
hip:add_child(h_t_joint_l)
h_t_joint_l:translate(-0.5, 0.0, 0.0)

thigh_l = gr.sphere('thigh_l')
h_t_joint_l:add_child(thigh_l)
thigh_l:set_material(blue)
thigh_l:translate(0.0, -1.0, 0.0)
thigh_l:scale(0.3, 1.0, 0.3)

t_c_joint_l = gr.joint('t_c_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
thigh_l:add_child(t_c_joint_l)
t_c_joint_l:translate(0.0, -1.0, 0.0)

calf_l = gr.sphere('calf_l')
t_c_joint_l:add_child(calf_l)
calf_l:set_material(blue)
calf_l:translate(0.0, -0.9, 0.0)
calf_l:scale(0.2, 1.0, 0.2)

c_f_joint_l = gr.joint('c_f_joint_l', {-90.0, 0.0, 90.0}, {-90.0, 0.0, 90.0})
calf_l:add_child(c_f_joint_l)
c_f_joint_l:translate(0.0, -1.0, 0.0)

foot_l = gr.sphere('foot_l')
c_f_joint_l:add_child(foot_l)
foot_l:set_material(green)
foot_l:translate(0.0, 0.0, 0.5)
foot_l:scale(0.2, 0.2, 0.5)

--  s0 = gr.sphere('s0')
--  rootnode:add_child(s0)
--  s0:set_material(white)
--  
--  s1 = gr.sphere('s1')
--  rootnode:add_child(s1)
--  s1:scale(0.1, 2.0, 0.1)
--  s1:set_material(red)
--  
--  s2 = gr.sphere('s2')
--  rootnode:add_child(s2)
--  s2:rotate('z', -90.0)
--  s2:translate(2.0, -2.0, 0.0)
--  s2:scale(0.1, 2.0, 0.1)
--  s2:set_material(blue)
--  
--  s3 = gr.sphere('s3')
--  rootnode:add_child(s3)
--  s3:scale(0.1, 0.1, 2.0)
--  s3:translate(0.0, -2.0, 2.0)
--  s3:set_material(green)
--  
-- rootnode:translate(-0.75, 0.25, -10.0)
-- rootnode:rotate('y', -20.0)

return rootnode
