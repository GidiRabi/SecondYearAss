from typing import Dict, Set

from CostomExcption import UsernameAlreadyExistsError, UserDoesNotExistError, NotLoginError, InvalidCredentialsError
from User import User


class SocialNetwork:
    """
    A class representing a social network.
    The social network is a singleton.
    """
    __instance = None
    __is_initialized = False

    @staticmethod
    def get_instance():
        if SocialNetwork.__instance is None:
            raise ValueError("The social network was not created yet")
        return SocialNetwork.__instance

    def __new__(cls, name: str):
        if cls.__instance is None:
            cls.__instance = super().__new__(cls)
        return cls.__instance

    def __init__(self, name: str):
        if self.__is_initialized:
            return  # already initialized, not need to initialize again
        self._name = name
        self._users: Dict[str, User] = dict()
        self._logged_in_users: Set[User] = set()

        self.__is_initialized = True  # update the flag to True

        print(f"The social network {name} was created!")

    def sign_up(self, username: str, password: str) -> User:
        if username in self._users:
            raise UsernameAlreadyExistsError("Username already exists")

        user = User(username, password)
        self._users[username] = user
        self._logged_in_users.add(user)
        return user

    def log_out(self, username: str) -> None:
        if username not in self._users:
            raise UserDoesNotExistError("Username does not exist")

        if self._users[username] not in self._logged_in_users:
            raise NotLoginError("Can't log out a user that is not logged in")

        # remove user from logged_in_users
        self._logged_in_users.remove(self._users[username])
        print(f"{username} disconnected")

    def log_in(self, username: str, password: str) -> None:
        if username not in self._users or self._users[username].compare_password(password) is False:
            raise InvalidCredentialsError("Invalid credentials")

        user = self._users[username]

        self._logged_in_users.add(user)
        print(f"{username} connected")

    def is_user_logged_in(self, user: User) -> bool:
        return user in self._logged_in_users

    def __str__(self):
        users = "\n".join([str(u) for u in self._users.values()])
        return f'{self._name} social network:\n{users}'

    def __repr__(self):
        return self.__str__()
