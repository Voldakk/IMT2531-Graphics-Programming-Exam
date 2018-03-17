#pragma once

#include <map>

#include "GL/glew.h"
#include "glm/glm/glm.hpp"

// Holds animation data for a Sprite
struct Animation
{
	// Which order the sprites should be played in
	enum PlayDirection
	{
		/// <summary>The sprites are played starting top left in the sprite sheet. The first row is the played left to right, followed by the next row below, the third row, ect</summary>
		Horizontal,
		/// <summary>The sprites are played starting top left in the sprite sheet. The first column is the played top to bottom, followed by the next column to the right, the third column, ect</summary>
		Vertical
	};

	// How the animation loops
	enum LoopMode
	{
		/// <summary>When the animation reaches the last frame it skips back to the first frame and starts again</summary>
		Loop,
		/// <summary>When the animation reaches the last frame it plays in reverse order back to the first frame and then starts again</summary>
		Bounce,
		/// <summary>Stops at the last frame</summary>
		None
	};

public:
	glm::uvec2 start;
	glm::uvec2 size;
	int numSprites;
	PlayDirection dir;
	LoopMode loopMode;
};

// A 2D graphics object to be displayed on screen
class Sprite
{
public:

	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	glm::vec4 tint;

	/// <summary>Constructor. Sets default values</summary>
	Sprite();

	/// <summary>Initializes resources shared among all Sprite objects
	/// <para>Creates VAP, VBO, EBO and shader program, buffer data and gets atribute locations</para>
	/// </summary>
	static void Init();

	/// <summary>Loads an image to be used as a sprite sheet</summary>
	/// <param name="path">The path to the image file</param>  
	/// <param name="numSprites">The number of rows and columns in the sprite sheet</param>  
	void LoadSpriteSheet(std::string path, glm::vec2 numSprites);

	/// <summary>Loads an image to be used as a tint mask</summary>
	/// <param name="path">The path to the image file</param>  
	void LoadTintMask(std::string path);

	/// <summary>Updates the sprites animations. Should be called every frame the sprite is active</summary>
	/// <param name="deltaTime">The time in seconds between frames</param>  
	virtual void Update(float deltaTime);

	/// <summary>Renders the sprite to the screen</summary>
	virtual void Render();

	/// <summary>Sets the sprites animation playback fps</summary>
	/// <param name="fps">The number of frames per second</param>  
	void SetFPS(float fps);

	/// <summary>Adds an animation to the sprite</summary>
	/// <param name="name">The name used to refference the animation</param>  
	/// <param name="start">The start index of the animation in the sprite sheet</param>  
	/// <param name="size">The number of sprites inthe x and y dirextion on the spritesheet to be played in the animation</param>  
	/// <param name="numSprites">The total number of sprites to be played</param>  
	/// <param name="dir">Which order the sprites should be played in</param>  
	/// <param name="loopMode">The animations loop mode</param>
	/// <seealso cref="Animation::PlayDirection"/>
	/// <seealso cref="Animation::LoopMode"/>
	void AddAnimation(std::string name, glm::uvec2 start, glm::uvec2 size = glm::uvec2(1, 1), int numSprites = 1, Animation::PlayDirection dir = Animation::PlayDirection::Horizontal, Animation::LoopMode loopMode = Animation::LoopMode::Loop);
	
	/// <summary>Sets the currently playing animation</summary>
	/// <param name="name">The name of the animation</param>  
	void SetAnimation(std::string name);

	/// <summary>Checks if a point is contained within the sprites bounding box</summary>
	/// <param name="point">The position of the point</param>  
	/// <returns>Returns true if the point is within the bounding box, false if not</returns>  
	bool ContainsPoint(glm::vec2 point);

	/// <summary>Checks if this sprites bounding box overlaps another sprites bounding box</summary>
	/// <param name="other">The other sprite</param>  
	/// <returns>Returns true if the bounding boxes overlap, false if not</returns>  
	bool Overlap(Sprite &other);

private:

	int currentSprite;
	int currentPlayDir;
	glm::uvec2 numSprites;
	
	float framesPerSec;
	double counter;

	std::string currentAnimation;
	std::map<std::string, Animation> animations;
	
	GLuint texture;
	GLuint tintMask;

	/// <summary>Updates the verteces UV coordinates to match the animation</summary>
	/// <param name="s">The current animation</param>  
	void UpdateVertices(Animation & s);
};