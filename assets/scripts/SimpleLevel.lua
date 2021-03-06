SimpleLevel = {
    assets = {
        [0] = { type="texture", id = "simple-tileset", file = "./assets/tilemaps/simple_overworld_tileset.png" },
        [1] = { type="texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
        [2] = { type="texture", id = "simple-player-texture", file = "./assets/images/simple_character_v2.png" }
    },

    tiledMap = {
        file = "./assets/tilemaps/simple.json",
        scale = 2,
    },

    entities = {
        [0] = {
            name = "player",
            layer = 4,
            components = {
                transform = {
                    position = {
                        x = 240,
                        y = 106
                    },
                    velocity = {
                        x = 0,
                        y = 0
                    },
                    width = 32,
                    height = 32,
                    scale = 1,
                    rotation = 0
                },
                sprite = {
                    textureAssetId = "chopper-texture",
                    animated = true,
                    frameCount = 2,
                    animationSpeed = 90,
                    hasDirections = true,
                    fixed = false
                },
                collider = {
                    tag = "PLAYER"
                },
                input = {
                    keyboard = {
                        up = "w",
                        left = "a",
                        down = "s",
                        right = "d",
                        shoot = "space"
                    }
                }
            }
        },
        [1] = {
            name = "player2",
            layer = 4,
            components = {
                transform = {
                    position = {
                        x = 340,
                        y = 106
                    },
                    velocity = {
                        x = 0,
                        y = 0
                    },
                    width = 16,
                    height = 16,
                    scale = 2,
                    rotation = 0
                },
                asepriteSprite = {
                    file = "./assets/images/simple_character_v2.json",
                    textureAssetId = "simple-player-texture",
                    fixed = false
                }
            }
        }
    }
}