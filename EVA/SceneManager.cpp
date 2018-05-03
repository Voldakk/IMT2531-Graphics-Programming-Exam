#include "SceneManager.hpp"

namespace EVA
{

	std::vector<std::shared_ptr<Scene>> SceneManager::m_Scenes;
	std::vector<Scene*> SceneManager::m_UnloadScenes;

	std::shared_ptr<Scene> SceneManager::CreateScene(std::string path)
	{
		std::cout << "SceneManager::CreateScene - Creating scene: " << path << " \n";
		auto scene = std::make_shared<Scene>(path);

		scene->Awake();
		scene->Start();

		LoadScene(scene);

		return scene;
	}

	void SceneManager::LoadScene(const std::shared_ptr<Scene> &scene)
	{
		if (GetIndex(scene) == -1)
		{
			std::cout << "SceneManager::LoadScene - Loading scene: " << typeid(*scene).name() << " \n";
			m_Scenes.push_back(scene);
			scene->self = scene;
		}
	}

	void SceneManager::UnloadScene(const std::shared_ptr<Scene> &scene)
	{
		UnloadScene(scene.get());
	}

	void SceneManager::UnloadScene(Scene* scene)
	{
		const auto i = GetIndex(scene);
		if (i != -1)
		{
			std::cout << "SceneManager::UnloadScene - Unloading scene: " << typeid(*scene).name() << " \n";

			scene->Abort();
			m_UnloadScenes.push_back(scene);

			m_Scenes.erase(m_Scenes.begin() + i);
		}
	}

	void SceneManager::ChangeScene(const std::shared_ptr<Scene> &scene)
	{
		std::cout << "SceneManager::ChangeScene - Changing scene: " << typeid(*scene).name() << " \n";
		ClearScenes();

		LoadScene(scene);
	}

	void SceneManager::ChangeScene(const std::string& path)
	{
		std::cout << "SceneManager::ChangeScene - Changing scene: " << path << " \n";
		ClearScenes();

		CreateScene(path);
	}

	void SceneManager::Update(const float deltaTime)
	{
		auto scenesCopy = m_Scenes;
		for (auto &scene : scenesCopy)
		{
			if (scene->abort)
			{
				continue;
			}

			scene->Update(deltaTime);
		}

		for (auto& scene : m_UnloadScenes)
		{
			scene->self.reset();
		}

		m_UnloadScenes.clear();
	}

	void SceneManager::LateUpdate()
	{
		auto scenesCopy = m_Scenes;
		for (auto &scene : scenesCopy)
		{
			if (scene->abort)
			{
				continue;
			}

			scene->LateUpdate();
		}

		for (auto& scene : m_UnloadScenes)
		{
			scene->self.reset();
		}

		m_UnloadScenes.clear();
	}

	void SceneManager::Render()
	{
		for (auto &scene : m_Scenes)
		{
			scene->Render();
		}
	}

	void SceneManager::OnScreenResize()
	{
		for (auto &scene : m_Scenes)
		{
			scene->OnScreenResize();
		}
	}

	int SceneManager::GetIndex(const std::shared_ptr<Scene> &scene)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (scene == m_Scenes[i])
				return i;
		}
		return -1;
	}

	int SceneManager::GetIndex(const Scene* scene)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (scene == m_Scenes[i].get())
				return i;
		}
		return -1;
	}

	void SceneManager::ClearScenes()
	{
		for (int i = m_Scenes.size() - 1; i >= 0; --i)
		{
			UnloadScene(m_Scenes[i]);
		}
	}
}