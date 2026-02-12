extends Node

@onready var network_manager: GDNetworkManager= $GDNetworkManager
@export var port: int = 5000
@export var msg: String = "Hello world"
@export var ip: String = "0.0.0.0"

func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example)
	network_manager.Bind(port)
	network_manager.Send(ip, port, msg.to_ascii_buffer())
	

func _process(_delta: float) -> void:
	network_manager.Receive()


func _on_gd_network_manager_packet_received(sender_ip: String, sender_port: int, data: PackedByteArray) -> void:
	print("Sender IP = ", sender_ip, "; port used = ", sender_port, "; data received = ", data.get_string_from_ascii())
