#pragma once
#include <vector>
#include <array>
#include <bitset>
#include <string>
#include <functional>
using namespace std;
class Entity;
class Component;

inline size_t getComponentTypeID()
{
	static size_t lastID = 0;
	return lastID++;
}
template<typename T>
inline size_t getComponentTypeID() noexcept
{
	static size_t typeID = getComponentTypeID();
	return typeID;
}
constexpr size_t maxComponents = 32;
using ComponentBitset = bitset<maxComponents>;
using ComponentArray = array<Component*, maxComponents>;
class Component
{
public:
	Entity *entity=nullptr;
	virtual void init() = 0;
	virtual void update()=0;
	virtual ~Component() {}
private:
	
};
class Entity
{
public:
	Entity():m_active(true)
	{}
	Entity(std::string i_Name) :m_Name(i_Name),m_active(true) {}
	std::string GetName()
	{
		return m_Name;
	}
	bool& getactive()
	{
		return m_active;
	}
	template<typename T>
	bool HasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}
	template<typename T>
	void AddComponent(T *i_Component)
	{
		if (i_Component)
		{
			
			i_Component->entity = this;
			m_Components.push_back(i_Component);
			componentArray[getComponentTypeID<T>()] = i_Component;
			componentBitset[getComponentTypeID<T>()] = true;
		}
	}
	template<typename T>
	T* getComponent()
	{
		T*  ptr = static_cast<T*>(componentArray[getComponentTypeID<T>()]);
		return ptr;
	}
	void init()
	{
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			m_Components[i]->init();
		}
	}
	void update()
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->update();
		}
	}
	template<typename Component>
	void updateCom()
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			if (auto component = dynamic_cast<Component*>(m_Components[i]))
			{
				component->update();
			}
		}
	}
	~Entity()
	{
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			delete m_Components[i];
		}
	}
private:
	std::string               m_Name;
	bool                      m_active;
	std::vector<Component*>   m_Components;	
	ComponentArray            componentArray;
	ComponentBitset           componentBitset;
};
class Manager
{
public:
	Manager(const Manager& i_Other) = delete;
	Manager& operator=(const Manager& i_Other) = delete;
	static Manager& Get()
	{
		static Manager* Instance = new Manager(); 
		return *Instance;
	}
	std::unique_ptr<Entity>& AddEntity()
	{
		Entity* e(new Entity());
		unique_ptr<Entity> uPtr{ e };
		return m_Entities.emplace_back(move(uPtr));

	}
	void init()
	{
		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i])
				m_Entities[i]->init();
		}
	}
	void update()
	{
		refresh();
		for (size_t i = 0; i < m_Entities.size(); i++)
		{	
			if(m_Entities[i])
				m_Entities[i]->update();
		}
	}
	template<typename Component>
	void updateCom()
	{
		refresh();
		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i])
				m_Entities[i]->updateCom<Component>();
		}
	}
	void refresh()
	{
		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i]->getactive() == false)
			{
				//m_Entities[i].Acquire()->~Entity();
				m_Entities.erase(m_Entities.begin()+i);
				
			}
		}
	}
	static void Shutdown()
	{
		delete &Get();
	}
	~Manager()
	{
		
	}
private:
	Manager() {}
	vector<std::unique_ptr<Entity>>        m_Entities;
};

