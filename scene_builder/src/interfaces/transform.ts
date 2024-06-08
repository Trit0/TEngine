export class Transform {
    translation: Vec3;
    rotation: Vec3;
    scale: Vec3;

    constructor(translation: Vec3, rotation: Vec3, scale: Vec3) {
        this.translation = translation;
        this.rotation = rotation;
        this.scale = scale;
    }
}

export interface Vec3 {
    x: number;
    y: number;
    z: number;
}