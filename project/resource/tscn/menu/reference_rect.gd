extends ReferenceRect


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if(self.border_color.r == 1):
		self.border_color.g = maxf(0, self.border_color.g - 1*delta)
		self.border_color.b = maxf(0, self.border_color.g - 1*delta)
	pass

func _gui_input(event: InputEvent) -> void:
	if(event is InputEventMouseMotion):
		self.border_color.b=1;
		self.border_color.g=1;
		
	if(event is InputEventMouseButton):
		if(event.button_index == MouseButton.MOUSE_BUTTON_LEFT or event.button_mask == MouseButton.MOUSE_BUTTON_RIGHT):
			if(event.pressed): self.border_color=Color(0, 1, 1, 1)
			else: self.border_color=Color(1, 1, 1, 1)
			

func _unhandled_input(event: InputEvent) -> void:
	if(event is InputEventKey):
		if(event.keycode == KEY_ESCAPE and event.pressed):
			get_tree().quit();
			
