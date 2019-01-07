#!/usr/bin/env python3
import grpc, random, sys, threading, time
from cube_demo_pb2_grpc import CubePhysicsDemoStub
from cube_demo_pb2 import *

# Reads a single unbuffered keypress from stdin
# (Adapted from the code example provided here: <https://www.raspberrypi.org/forums/viewtopic.php?t=69046#p502895>)
def getch():
	import sys, tty, termios
	settings = termios.tcgetattr(0)
	settings[3] &= ~termios.ICANON
	termios.tcsetattr(0, termios.TCSANOW, settings)
	ch = sys.stdin.read(1)
	return ch

# The port number of the gRPC server
SERVER_PORT = 50051

# Print our instructions for the user
print('Cube Physics gRPC Demo Client')
print()
print('Instructions:')
print('- Press R to reset the cube position')
print('- Press F to apply a random physics force to the cube')
print('- Press Q to quit the client')
print()

# Attempt to connect to the server
channel = grpc.insecure_channel('127.0.0.1:{}'.format(SERVER_PORT))
stub = CubePhysicsDemoStub(channel)
stub.ResetCubeTransform(Empty())

# Keep track of whether the user has pressed the "Q" key
shouldQuit = False

# Our interactive input loop
def inputLoop():
	global shouldQuit
	while shouldQuit == False:
		key = getch()
		if key == 'q':
			shouldQuit = True
		elif key == 'r':
			stub.ResetCubeTransform(Empty())
		elif key == 'f':
			force = Vector3D()
			force.x = random.uniform(-1000.0, 1000.0)
			force.y = random.uniform(-1000.0, 1000.0)
			force.z = random.uniform(-1000.0, 1000.0)
			stub.ApplyForceToCube(force)

# Start our interactive input thread
inputThread = threading.Thread(target=inputLoop)
inputThread.start()

# Continuously retrieve and display the cube's current transform while we accept user input
print('Current cube transform:')
while shouldQuit == False:
	transform = stub.GetCubeTransform(Empty())
	sys.stdout.write("\033[K")
	print('Pos: [{:.4f},{:.4f},{:.4f}], Rot: [{:.4f},{:.4f},{:.4f}], Scale: [{:.4f},{:.4f},{:.4f}]'.format(
		transform.position.x,
		transform.position.y,
		transform.position.z,
		transform.rotation.x,
		transform.rotation.y,
		transform.rotation.z,
		transform.scale.x,
		transform.scale.y,
		transform.scale.z
	), end='\r')
	time.sleep(1/30)

# Wait for the input thread to complete
inputThread.join()
print()
