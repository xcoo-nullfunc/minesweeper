extends Camera2D

@export var speed : float = 150
@export var quick_move_multi : float = 3
@export var move_sensitive : float = 1
@export var reverse_move : bool = false
var zoomr : float = 1
var dire : Vector2
#var mouse_site : Vector2

func _ready() -> void:
	dire = Vector2(0, 0)
	self.position += get_viewport_rect().size / 2 - Vector2(20, 100)

func _process(delta: float) -> void:
	if(Input.is_key_pressed(KEY_SHIFT)):
		self.position += delta * dire * speed * quick_move_multi / self.zoom.x
	else:
		self.position += delta * speed * dire / self.zoom.x;
	if(Input.is_mouse_button_pressed(MOUSE_BUTTON_XBUTTON1)):
		self.zoomr -= 1 * delta
		set_zoomr()
	if(Input.is_mouse_button_pressed(MOUSE_BUTTON_XBUTTON2)):
		self.zoomr += 1 * delta
		set_zoomr()
	

func _input(event: InputEvent) -> void:
	#鼠标中键移动
	if(event is InputEventMouseMotion):
		if(Input.is_mouse_button_pressed(MOUSE_BUTTON_MIDDLE)):
				var move = event.relative * self.move_sensitive / self.zoom.x
				if(self.reverse_move): self.position -= move
				else: self.position += move
		
	

func _unhandled_input(event: InputEvent) -> void:
	if(event is InputEventKey):
		if(event.pressed):
			if(event.keycode == KEY_W):
				self.dire.y = -1;
			elif(event.keycode == KEY_S):
				self.dire.y = 1;
			elif(event.keycode == KEY_A):
				self.dire.x = -1;
			elif(event.keycode == KEY_D):
				self.dire.x = 1;
			elif(event.keycode == KEY_SPACE):
				self.zoom = Vector2(1, 1)
				self.position = get_viewport_rect().size / 2 - Vector2(20, 100)
			
				
		else:
			if(event.keycode == KEY_W and self.dire.y < 0 or event.keycode == KEY_S and self.dire.y > 0):
				self.dire.y = 0;
			elif(event.keycode == KEY_A and self.dire.x < 0 or event.keycode == KEY_D and self.dire.x > 0):
				self.dire.x = 0;
				
	elif(event is InputEventMouseButton and event.device >= 0):
		if(event.pressed):
			if(event.button_index == MOUSE_BUTTON_WHEEL_DOWN):
				self.zoomr -= 0.1;
				set_zoomr()
				#self.resized.emit()
				
			elif(event.button_index == MOUSE_BUTTON_WHEEL_UP):
				self.zoomr += 0.1;
				set_zoomr()
				#self.resized.emit()

	elif(event is InputEventScreenDrag):
		if(event.index == 0):
			var move_relative = event.relative * move_sensitive / self.zoom.x
			if(reverse_move):
				move_relative *= -1
			self.position += move_relative

func set_zoomr() -> void:
	if(self.zoomr > $"../CanvasLayer/HSlider_zoomr".max_value): self.zoomr = $"../CanvasLayer/HSlider_zoomr".max_value
	elif(self.zoomr < $"../CanvasLayer/HSlider_zoomr".min_value): self.zoomr = $"../CanvasLayer/HSlider_zoomr".min_value
	self.zoom = Vector2(pow(zoomr, 2), pow(zoomr, 2))
	$"../CanvasLayer/HSlider_zoomr".value = self.zoomr

func accept_zoomr(zoom_r : float) -> void:
	self.zoomr = zoom_r
	self.zoom = Vector2(pow(zoomr, 2), pow(zoomr, 2))

func set_move_speed(ms : float) -> void:
	self.speed = ms

func set_quick_move_multi(multi : float) -> void:
	self.quick_move_multi = multi

func set_mouse_sensitive(sensitive : float) -> void:
	self.move_sensitive = sensitive

func set_reserve_mouse(reserve : bool) -> void:
	self.reverse_move = reserve
