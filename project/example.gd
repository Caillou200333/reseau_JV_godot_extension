extends Node

@onready var network_manager: GDNetworkManager= $GDNetworkManager
@onready var entity_manager: GDEntityManager= $GDEntityManager
@onready var input_manager: Node2D= $GDInputManager
@export var port: int = 5000
@export var server_port: int = 8080
@export var msg: String = "Ping"
@export var ip: String = "0.0.0.0"

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

func _physics_process(_delta):
	var input : GDInput = input_manager.build_input()
	GDInputMessage.add_input(input)
	var input_message := GDInputMessage.new()
	network_manager.Send(ip, server_port, input_message)

func _process(_delta: float) -> void:
	network_manager.Receive()
	pass

func _on_gd_network_manager_packet_received(_sender_ip: String, _sender_port: int, message: GDBaseMessage) -> void:
	#print("Sender IP = ", sender_ip, "; port used = ", sender_port)
	if message is GDHELOMessage:
		#var helo_msg = message as GDHELOMessage
		print("Helo !")
	if message is GDGameplayMessage:
		var gameplay_msg = message as GDGameplayMessage
		if not entity_manager.has(gameplay_msg.get_network_id()):
			var new_entity : GDEntity = entity_manager.create(gameplay_msg.get_network_id(), gameplay_msg.get_class_id())
			new_entity.init(gameplay_msg.get_x(), gameplay_msg.get_y())
			print("Spawn new entity : ", gameplay_msg.get_network_id(), " | ",
			gameplay_msg.get_class_id(), " | ", gameplay_msg.get_x(), " | ", gameplay_msg.get_y())
		else:
			var entity : GDEntity = entity_manager.get(gameplay_msg.get_network_id())
			entity.move(gameplay_msg.get_x(), gameplay_msg.get_y())
	if message is GDTextMessage:
		var text_msg = message as GDTextMessage
		print("Message texte reçu : ", text_msg.get_text())
