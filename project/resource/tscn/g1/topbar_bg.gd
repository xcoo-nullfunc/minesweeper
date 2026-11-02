extends ColorRect

enum sites{
	top,
	buttom,
	left,
	right
}

var site : sites = sites.top
var barWidth : float = 80

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	resize()
	pass # Replace with function body.


func resize() -> void:
	if(self.site == self.sites.top or self.site == self.sites.buttom):
		self.size.x = get_viewport_rect().size.x
		self.size.y = self.barWidth
	elif(self.site == self.sites.left or self.site == self.sites.right):
		self.size.x = self.barWidth
		self.size.y = get_viewport_rect().size.y
		
	if(self.site == self.sites.top):
		self.position.x = 0;
		self.position.y = 0;
	elif(self.site == self.sites.buttom):
		self.position.x = 0;
		self.position.y = get_viewport_rect().size.y - self.size.y
	elif(self.site == self.sites.left):
		self.position.y = 0;
		self.position.x = 0;
	elif(self.site == self.sites.right):
		self.position.y = 0;
		self.position.y = get_viewport_rect().size.x - self.size.x
		

func _on_camera_2d_resized() -> void:
	resize();
	pass # Replace with function body.

func setBarSite(setSite : sites) -> void:
	self.site = setSite
	self.resize()
