extends Node2D

const INPUT_UP    = 1 << 0
const INPUT_DOWN  = 1 << 1
const INPUT_LEFT  = 1 << 2
const INPUT_RIGHT = 1 << 3

func build_input() -> Array: # couple GDInput et Vector2 (velocity)
	var input = GDInput.new()
	var velocity = Vector2.ZERO
	if Input.is_action_pressed("move_up"):
		input.add_key(INPUT_UP)
		velocity.y -= 1
	if Input.is_action_pressed("move_down"):
		input.add_key(INPUT_DOWN)
		velocity.y += 1
	if Input.is_action_pressed("move_left"):
		input.add_key(INPUT_LEFT)
		velocity.x -= 1
	if Input.is_action_pressed("move_right"):
		input.add_key(INPUT_RIGHT)
		velocity.x += 1
	var aim = get_global_mouse_position()
	input.set_aim(aim.x, aim.y)
	return [input, velocity]
