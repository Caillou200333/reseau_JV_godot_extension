extends Node

@onready var network_manager: GDNetworkManager = $GDNetworkManager
@onready var entity_manager: GDEntityManager = $GDEntityManager
@onready var input_manager: Node2D = $GDInputManager
@onready var snapshot_manager: GDSnapShotManager = GDSnapShotManager.new()

@export var port: int = 5000
@export var server_port: int = 8080
@export var msg: String = "Ping"
@export var ip: String = "0.0.0.0"

@onready var is_connecting: bool = true
@onready var entity_controlled_id: int = -1
@onready var entity_controlled_speed: float = 0.
@export var delay: float = 0.1 # 100 ms
@export var jitter: int = 5

var delta_max = 0

func _ready() -> void:
	var args = OS.get_cmdline_args()
	for i in range(args.size()):
		if args[i] == "--port" and i+1 < args.size():
			port = int(args[i+1])
	print("Client listening on port ", port)
	network_manager.Bind(port)
	network_manager.StartRTT(ip, server_port)
	var txt_message := GDTextMessage.new()
	txt_message.init(msg)
	network_manager.Send(ip, server_port, txt_message)
	var helo_message := GDHELOMessage.new()
	network_manager.Send(ip, server_port, helo_message)

func _physics_process(delta):
	if not entity_manager.has(entity_controlled_id): # if no entity assigned, no input to send
		return
	var input_build = input_manager.build_input()
	var input : GDInput = input_build[0]
	#var velocity : Vector2 = input_build[1]
	GDInputMessage.add_input(input)
	var input_message := GDInputMessage.new()
	network_manager.Send(ip, server_port, input_message)
	"""
	var entity_controlled : GDEntity = entity_manager.get(entity_controlled_id)
	print(entity_controlled.position, " | POSITION | ", entity_controlled.global_position)
	var new_pos : Vector2 = entity_controlled.position + velocity * entity_controlled_speed * delta
	entity_controlled.move(int(new_pos.x), int(new_pos.y))
	"""

func _process(delta: float) -> void:
	network_manager.Receive()
	if is_connecting:
		var helo_message := GDHELOMessage.new()
		network_manager.Send(ip, server_port, helo_message)
		return
	if snapshot_manager.is_buffer_ready():
		if delta_max < delta:
			delta_max = delta
			print("New Delta Max = ", delta_max)
		snapshot_manager.load_current_snapshot(delta)
		while (!snapshot_manager.is_empty()):
			if not entity_manager.has(snapshot_manager.get_network_id()):
				var new_entity : GDEntity = entity_manager.create(snapshot_manager.get_network_id(), snapshot_manager.get_class_id())
				new_entity.init(snapshot_manager.get_x(), snapshot_manager.get_y())
				print("Spawn new entity : ", snapshot_manager.get_network_id(), " | ",
				snapshot_manager.get_class_id(), " | ", snapshot_manager.get_x(), " | ", snapshot_manager.get_y())
			else:
				#if (entity_controlled_id != snapshot_manager.get_network_id()): # entity conntrolled is moved by the client
				var entity : GDEntity = entity_manager.get(snapshot_manager.get_network_id())
				entity.move(snapshot_manager.get_x(), snapshot_manager.get_y())
			snapshot_manager.next()
	pass

func _on_gd_network_manager_packet_received(_sender_ip: String, _sender_port: int, message: GDBaseMessage) -> void:
	#print("Sender IP = ", sender_ip, "; port used = ", sender_port)
	if message is GDHELOMessage:
		var helo_msg = message as GDHELOMessage
		print("frame = ", helo_msg.get_frame_length(), " | entity = ", helo_msg.get_entity_controlled_id())
		snapshot_manager.set_frame_length(helo_msg.get_frame_length())
		print("frame_length = ", snapshot_manager.get_frame_length())
		print("Frame delay = ", int(delay / snapshot_manager.get_frame_length()) + jitter)
		snapshot_manager.set_max_frame(int(delay / snapshot_manager.get_frame_length()) + jitter)
		entity_controlled_id = helo_msg.get_entity_controlled_id()
		entity_controlled_speed = helo_msg.get_entity_controlled_speed()
		print("Speed = ", entity_controlled_speed)
		is_connecting = false
	if message is GDGameplayMessage:
		var gameplay_msg = message as GDGameplayMessage
		gameplay_msg.save_snapshot(snapshot_manager)
	if message is GDTextMessage:
		var text_msg = message as GDTextMessage
		print("Message texte reçu : ", text_msg.get_text())
