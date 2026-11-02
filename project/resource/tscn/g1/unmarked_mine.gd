extends Label

func _ready() -> void:
	self.size.x = get_viewport_rect().size.x - 30;

func _physics_process(delta: float) -> void:
	self.text = str($"../../MST".get_unmarked_mine())
	pass
