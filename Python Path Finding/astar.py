import math

#Container for each node in the search space
class Node(object):
	def __init__(self):
		self.position = [0, 0]

		self.connections = []
		self.f = 999999999
		self.g = 999999999
		self.is_searched = False

#MODE_GRID: Assume can only move up/down/left/right
MODE_GRID = 0

#MODE_DIRECT: Assume can move directly between nodes
MODE_DIRECT = 1

current_mode = 1
def setMode(mode):
	global current_mode
	current_mode = mode

#Set up the starting node
def initSearch(nodes, start_id, goal_id):
	nodes[start_id].is_searched = True
	nodes[start_id].g = 0

	if current_mode == MODE_GRID:
		nodes[start_id].f = math.fabs(nodes[start_id].position[0] - nodes[goal_id].position[0]) \
							+ math.fabs(nodes[start_id].position[1] - nodes[goal_id].position[1])
	elif current_mode == MODE_DIRECT:
		nodes[start_id].f = math.hypot(nodes[start_id].position[0] - nodes[goal_id].position[0],
							nodes[start_id].position[1] - nodes[goal_id].position[1])

#Search a new node
def searchStep(nodes, start_id, goal_id):

	computed = []
	#Compute all neighbouring nodes
	for node in nodes:
		if node.is_searched:
			for test in node.connections:
				if not test.is_searched:

					#Compute node
					#Distance from start to node
					test.g = min(node.g + math.fabs(test.position[0] - node.position[0]) + math.fabs(test.position[1] - node.position[1]), test.g)

					#Distance from start to node to goal
					if current_mode == MODE_GRID:
						test.f = min(test.f, test.g + math.fabs(test.position[0] - nodes[goal_id].position[0])
										+ math.fabs(test.position[1] - nodes[goal_id].position[1]))
					elif current_mode == MODE_DIRECT:
						test.f = min(test.f, test.g + math.hypot(test.position[0] - nodes[goal_id].position[0],
										test.position[1] - nodes[goal_id].position[1]))

					computed.append(test)

	#Compute node with minimum value for f
	min_node = None
	for node in computed:
		if min_node == None or min_node.f > node.f:
			min_node = node
		elif min_node.f == node.f and node.g > min_node.g:
			min_node = node

	#Flag node as searched
	min_node.is_searched = True

	#Return True if done
	return nodes[goal_id].is_searched

#Get the path after search is complete
def getPath(nodes, start_id, goal_id):
	current = nodes[goal_id]
	traversal = [nodes[goal_id]]

	while current != nodes[start_id]:

		#Search for neighbouring node with minimum f value
		min_node = None
		for node in current.connections:
			if node.is_searched and (min_node == None or min_node.g > node.g):
				min_node = node

		#Update traversal & current
		traversal.append(min_node)
		current = min_node

	return traversal