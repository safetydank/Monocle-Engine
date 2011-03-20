#include "Entity.h"
#include "Collision.h"
#include "Graphics.h"

namespace Monocle
{
	Entity::Entity()
		: scene(NULL), collider(NULL), graphic(NULL), layer(0), depth(0.0f), scale(Vector2::one), rotation(0.0f), color(Color::white)
		//, willDie(false)
	{
	}

	Entity::~Entity()
	{

	}

	void Entity::Added()
	{

	}

	void Entity::Removed()
	{

	}

	void Entity::Update()
	{
		for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Update();
		}
	}

	void Entity::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position.x, position.y, depth);
		Graphics::Rotate(rotation, 0, 0, 1);
		Graphics::Scale(scale);

		for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Render();
		}

		if (graphic != NULL)
		{
			Graphics::SetColor(color);
			graphic->Render();
		}

		Graphics::PopMatrix();
	}

	const std::string& Entity::GetTag(int index)
	{
#ifdef DEBUG
		//Error: If the tag index to get is out of bounds
		if (index >= tags.size())
			Debug::Log("ERROR: Tag index out of bounds.");
#endif
		return tags[index];
	}

	void Entity::AddTag(const std::string& tag)
	{
#ifdef DEBUG
		//Error: If the entity already has that tag
		if (HasTag(tag))
			Debug::Log("ERROR: Duplicate tag added to entity.");
#endif
		tags.push_back(tag);
		if (scene != NULL)
			scene->EntityAddTag(this, tag);
	}

	void Entity::RemoveTag(const std::string& tag)
	{
#ifdef DEBUG
		//Error: If the entity doesn't have that tag
		if (!HasTag(tag))
			Debug::Log("ERROR: Removing tag from an entity that doesn't have that tag.");
#endif
		for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
		{
			if ((*i).compare(tag) == 0)
			{
				tags.erase(i);
				break;
			}
		}
		if (scene != NULL)
			scene->EntityRemoveTag(this, tag);
	}

	bool Entity::HasTag(const std::string& tag)
	{
		for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
		{
			if ((*i).compare(tag) == 0)
				return true;
		}

		return false;
	}

	int Entity::GetNumberOfTags()
	{
		return static_cast<int>(tags.size());
	}

	int Entity::GetLayer()
	{
		return layer;
	}

	void Entity::SetLayer(int layer)
	{
		this->layer = layer;

		//TODO: get scene to re-sort me
	}

	bool Entity::IsLayer(int layer)
	{
		return this->layer == layer;
	}

	///TODO: enqueue for safety
	// add an entity as a child
	void Entity::Add(Entity *entity)
	{
		children.push_back(entity);
	}

	///TODO: enqueue for safety
	void Entity::Remove(Entity *entity)
	{
		children.remove(entity);
	}

	void Entity::SetCollider(Collider *collider)
	{
		if (collider == NULL && this->collider != NULL)
		{
			Collision::RemoveCollider(collider);
			collider = NULL;
		}
		else if (this->collider != NULL)
		{
			Debug::Log("Error: Entity already has a collider.");
		}
		else
		{
			this->collider = collider;
			Collision::RegisterColliderWithEntity(collider, this);
		}
	}

	Collider* Entity::GetCollider()
	{
		return collider;
	}

	Collider* Entity::Collide(const std::string &tag)
	{
		return Collision::Collide(this, tag);
	}

	/*
	RectangleCollider* Entity::AddRectangleCollider(float width, float height, const Vector2 &offset)
	{
		return Collision::AddRectangleCollider(this, width, height, offset);
	}
	*/

	void Entity::SendNoteToScene(const std::string &note)
	{
		if (scene)
		{
			scene->ReceiveNote(note, this);
		}
	}

	void Entity::SendNote(const std::string &tag, const std::string &note)
	{
		if (scene)
		{
			scene->RelayNoteTo(tag, note, this);
		}
	}

	void Entity::ReceiveNote(const std::string &tag, const std::string &note, Entity *fromEntity)
	{

	}

	void Entity::SetGraphic(Graphic *graphic)
	{
		// not sure yet
		/*
		if (graphic == NULL && this->graphic)
		{
			delete this->graphic;
		}
		*/
		if (this->graphic != NULL)
		{
			Debug::Log("Error: Entity already has a graphic.");
		}
		else
		{
			this->graphic = graphic;
		}
	}

	Graphic* Entity::GetGraphic()
	{
		return graphic;
	}

	/*
	void Entity::Die()
	{
		if (scene)
		{
			willDie = true;
			scene->Remove(this);
		}
		else
		{
			Debug::Log("Error: Entity is not in a scene. 'Die' will not function.");
		}
	}
	*/

	/*
	void Entity::CleanUp()
	{
		std::list<Entity*> toKill;
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->willDie)
			{
				toKill.push_back(*i);
			}
			(*i)->CleanUp();
		}

		for (std::list<Entity*>::iterator i = toKill.begin(); i != toKill.end(); ++i)
		{
			children.remove(*i);
		}
	}
	*/
}
