#ifndef USER_PREFERENCES
#define USER_PREFERENCES

class UserPreferences {

  private:
    struct UserSettings {
      String ssid;
      String pass;
    };
  public:
    UserSettings getPreferences();
    void setPreferences(String conf);

}

#endif

