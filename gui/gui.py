import sys
import pygame

class Boat:
    def __init__(self, x, y, boat_type, sprite, side):
        self.x = x
        self.y = y
        self.boat_type = boat_type
        self.sprite = sprite
        self.side = side

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def set_x(self, x):
        self.x = x

    def set_y(self, y):
        self.y = y

    def get_boat_type(self):
        return self.boat_type

    def get_sprite(self):
        return self.sprite

    def get_side(self):
        return self.side

SCREEN_WIDTH = 1650
SCREEN_HEIGHT = 850
BOAT_WIDTH = 300
BOAT_HEIGHT = 250
MAX_CHANNEL_WIDTH = SCREEN_WIDTH - 2 * BOAT_WIDTH
MIN_CHANNEL_WIDTH = 500
channel_length = MIN_CHANNEL_WIDTH
channel_position = (SCREEN_WIDTH / 2) - channel_length / 2
left_ocean = channel_position - 2 * BOAT_WIDTH
right_ocean = channel_position + channel_length + BOAT_WIDTH

def load_img(directory):
    img = pygame.image.load(directory)
    return img

try:
    skyImg = load_img("img/sky.png")
    oceanImg = load_img("img/ocean.png")
    boatImg = load_img("img/boat.png")
    fishingImg = load_img("img/fishing_boat.png")
    patrolImg = load_img("img/patrol_boat.png")
except pygame.error as e:
    print(f"Error loading image: {e}")
    sys.exit()

sky = pygame.transform.scale(skyImg, (SCREEN_WIDTH, SCREEN_HEIGHT))
ocean = pygame.transform.scale(oceanImg, (SCREEN_WIDTH, SCREEN_HEIGHT))

leftBoat = pygame.transform.scale(boatImg, (BOAT_WIDTH, BOAT_HEIGHT))
rightFishingBoat = pygame.transform.scale(fishingImg, (BOAT_WIDTH, BOAT_HEIGHT))
rightPatrolBoat = pygame.transform.scale(patrolImg, (BOAT_WIDTH, BOAT_HEIGHT))

rightBoat = pygame.transform.flip(leftBoat, True, False)
leftFishingBoat = pygame.transform.flip(rightFishingBoat, True, False)
leftPatrolBoat = pygame.transform.flip(rightPatrolBoat, True, False)

def move_right2left(screen, boat, x):
    if x > -BOAT_WIDTH:
        x -= 5
        if x <= channel_position - BOAT_WIDTH or x >= channel_position + channel_length:
            y = 555
        else:
            y = 430
        screen.blit(boat, (x,y))
    return x

def generate_simple_boat(side):
    if side < SCREEN_WIDTH / 2:
        b_side = "left"
        sprite = rightBoat
        init_y = 295
    else:
        b_side = "right"
        sprite = leftBoat
        init_y = 555
    return Boat(side,init_y,1, sprite, b_side)

def generate_fishing_boat(side):
    if side < SCREEN_WIDTH / 2:
        b_side = "left"
        sprite = rightFishingBoat
        init_y = 295
    else:
        b_side = "right"
        sprite = leftFishingBoat
        init_y = 555
    return Boat(side,init_y,2, sprite, b_side)

def generate_patrol_boat(side):
    if side < SCREEN_WIDTH / 2:
        b_side = "left"
        sprite = rightPatrolBoat
        init_y = 295
    else:
        b_side = "right"
        sprite = leftPatrolBoat
        init_y = 555
    return Boat(side,init_y,3, sprite, b_side)

def game_loop():
    pygame.init()

    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("Scheduling Ships")

    channel1 = pygame.Rect((channel_position, (SCREEN_HEIGHT / 2) + 50, channel_length, 50))
    channel2 = pygame.Rect((channel_position, (SCREEN_HEIGHT / 2) + 200, channel_length, 50))

    boat = None
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    boat = generate_simple_boat(left_ocean)
                elif event.key == pygame.K_RIGHT:
                    boat = generate_simple_boat(right_ocean)

        screen.fill((255, 255, 255))

        screen.blit(sky, (0,0))
        screen.blit(ocean, (0,0))

        pygame.draw.rect(screen, (153, 156, 159), channel1)
        pygame.draw.rect(screen, (153, 156, 159), channel2)

        if boat is not None:
            if boat.get_side() == "left" and boat.get_x() < SCREEN_WIDTH:
                boat.set_x(boat.get_x() + 5)
                if boat.get_x() <= channel_position - BOAT_WIDTH or boat.get_x() >= channel_position + channel_length:
                    boat.set_y(295)
                else:
                    boat.set_y(430)

            elif boat.get_side() == "right" and boat.get_x() > -BOAT_WIDTH:
                boat.set_x(boat.get_x() - 5)
                if boat.get_x() <= channel_position - BOAT_WIDTH or boat.get_x() >= channel_position + channel_length:
                    boat.set_y(555)
                else:
                    boat.set_y(430)


            screen.blit(boat.get_sprite(), (boat.get_x(), boat.get_y()))

        pygame.display.update()
    pygame.quit()

game_loop()