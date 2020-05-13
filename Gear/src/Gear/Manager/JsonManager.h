#pragma once

namespace Gear {

	class JsonAble
	{
	public:
		JsonAble() = default;
		JsonAble(const std::string& name)
			: Name(name)
		{}
		std::string Name;

		void SetName(const std::string& name)
		{
			Name = name;
		}
		virtual void Write(Json::Value& value) = 0;
		virtual void Read(const Json::Value& value) = 0;
	};

	class JsonManager
	{
	private:
		static JsonManager* s_Inst;

	public:
		static JsonManager* Get();
		static void Init();
		static void Destroy();

		template<typename T>
		bool TypeCheck()
		{
			bool isJsonable = std::is_base_of<JsonAble, T>::value;
			GR_CORE_ASSERT(isJsonable, "JsonManager::TypeCheck {0} Type is't derived from JsonAble!", typeid(T).name());
			return isJsonable;
		}

		void SetPath(const std::string& path)
		{
			bool isJson = false;

			int length = path.length();
			if (length > 5)
			{
				std::string filetype = path.substr(length - 5);
				if (filetype == ".json")
				{
					isJson = true;
				}
			}

			std::string checkpath = std::experimental::filesystem::current_path().string() + "\\" + path;
			if (isJson)
			{
				checkpath = checkpath.substr(0, checkpath.length() - 5);
			}
			
			if (!std::experimental::filesystem::is_directory(checkpath))
			{
				std::experimental::filesystem::create_directories(checkpath);
			}
		}

		std::string GetFullPath(const std::string& path, const std::string& name = "")
		{
			std::string fullPath = path;

			bool istDir = std::experimental::filesystem::is_directory(path);
			bool isFile = std::experimental::filesystem::is_regular_file(path);

			if (!isFile && !istDir)
			{
				return "";
			}

			if (istDir)
			{
				fullPath += ("\\" + name + ".json");
			}

			if (isFile)
			{
				int length = fullPath.length();
				if (length < 5)
				{
					return "";
				}
				std::string checkJson = fullPath.substr(length - 5);
				if (checkJson != ".json")
				{
					return "";
				}
			}
			return fullPath;
		}

		template<typename T>
		T* CreateFromRead(const std::string& path, const std::string& name)
		{
			TypeCheck<T>();
			T* ret = new T;
			ret->SetName(name);

			std::string fullpath = path + "\\" + name + ".json";
			if (!std::experimental::filesystem::is_regular_file(fullpath))
			{
				GR_CORE_ASSERT(false, "JsonManager::CreateFromRead {0} does't exist!", fullpath);
				return nullptr;
			}

			std::ifstream input(fullpath, std::ifstream::binary);
			if (!input)
			{
				GR_CORE_ASSERT(false, "JsonManager::CreateFromRead {0} does't exist!", fullpath);
				return nullptr;
			}

			Json::Reader reader;
			Json::Value root;
			reader.parse(input, root);  //json >> root; 
			input.close();

			ret->Read(root);
			return ret;
		}

		template<typename T>
		void Read(const std::string& path, T& data)
		{
			TypeCheck<T>();

			std::string fullpath = GetFullPath(path, data.Name);
			if (fullpath.empty() || !std::experimental::filesystem::is_regular_file(fullpath))
			{
				GR_CORE_ASSERT(false, "JsonManager::Read {0} does't exist!", fullpath);
				return;
			}

			std::ifstream input(fullpath, std::ifstream::binary);
			Json::Reader reader;
			Json::Value value;
			reader.parse(input, value);
			input.close();

			data.Read(value);
		}

		template<typename T>
		void Write(const std::string& path, T& data)
		{
			TypeCheck<T>();

			SetPath(path);

			std::string fullpath = path;
			if (std::experimental::filesystem::is_directory(path))
			{
				fullpath += ("\\" + data.Name + ".json");
			}

			Json::Value value;
			data.Write(value);

			std::ofstream ouput(fullpath, std::ios::out);

			ouput << value;
			ouput.close();
		}

		template<typename T>
		void WriteAll(const std::string& path, T& data)
		{
			Write(path, data);
		}

		template<typename T, typename... Types>
		void WriteAll(const std::string& path, T& data, Types... args)
		{
			Write(path, data);
		}

		//NOTE : Directory Must have same type of data
		template<typename T>
		std::vector<T*> ReadAll(const std::string& path)
		{
			TypeCheck<T>();
			std::vector<T*> ret;

			for (auto& p : std::experimental::filesystem::recursive_directory_iterator(path))
			{
				std::string path = p.path().string();
				int length = path.length();

				std::string filetype = path.substr(length - 5);
				if (filetype == ".json")
				{
					T* data = new T;
					auto tokken = Gear::Util::GetStrTokken(path, '\\');
					std::string name = tokken[tokken.size() - 1];
					data->SetName(name.substr(0, name.length() - 5));
					Read<T>(path, *data);

					ret.push_back(data);
				}
			}
			return ret;
		}
	};

}