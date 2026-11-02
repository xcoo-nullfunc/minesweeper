extends Area2D

var speed : float
var dire : Vector2

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	speed = 20
	dire = Vector2i(0, 0)
	monitoring = true
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	self.position += speed * delta * dire
	
	pass

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed:
		if(event.keycode == KEY_W):
			dire.y = -20
		elif(event.keycode == KEY_S):
			dire.y = 20
		elif(event.keycode == KEY_A):
			dire.x = -20
		elif(event.keycode == KEY_D):
			dire.x = 20;
	elif(event is InputEventKey and not event.pressed):
		if(event.keycode == KEY_W or event.keycode == KEY_S):
			dire.y = 0
		if(event.keycode == KEY_A or event.keycode == KEY_D):
			dire.x = 0
