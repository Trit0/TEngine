import { Scene } from "./interfaces/scene";
import { Transform } from "./interfaces/transform";
import { MAX_ENTITIES, randomInt } from "./index";

export function bunchOfCubes(scene: Scene) {
    for (let i = 0; i < MAX_ENTITIES; i++) {
        const scale = randomInt(1, 4);
        scene.entities.push({
            transform: new Transform({
                x: randomInt(-50, 150),
                y: randomInt(-50, 150),
                z: randomInt(-50, 150)
            }, {x: randomInt(0, 3), y: randomInt(0, 3), z: randomInt(0, 3)}, {
                x: scale,
                y: scale,
                z: scale
            }),
            model: "../models/box.obj",
            texture: randomInt(0, 1) == 1 ? "../textures/grass_block_side.png" : undefined
        });
    }
}

export function planeOfCubes(scene: Scene) {
    const rowLength = 3;
    const model = "../models/box.obj";
    const texture = "../textures/grass_block_side.png";
    const cubeSize = 6;
    const spacing = 3;
    for (let i = 0; i < 9; i++) {
        const scale = 3;
        const x = (i % rowLength);
        const z = (Math.floor(i / rowLength));
        scene.entities.push({
            transform: new Transform({
                x: (x * cubeSize) + (x * spacing),
                y: 20,
                z: (z * cubeSize) + (z * spacing)
            }, {x: 0, y: 0, z: 0}, {
                x: scale,
                y: scale,
                z: scale
            }),
            model,
            texture
        });
    }
}