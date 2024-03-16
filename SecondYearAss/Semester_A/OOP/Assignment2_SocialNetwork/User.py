# for type hinting
from __future__ import annotations

from typing import TYPE_CHECKING, Set, List

if TYPE_CHECKING:
    from posts.Post import Post

# end type hinting

from CostomExcption import NotLoginError, IllegalOperationError
from PostFactory import PostFactory


class User:
    _post_factory = PostFactory()

    def __init__(self, username: str, password: str):
        if len(password) < 4 or len(password) > 8:
            raise ValueError("Password must be 4 to 8 characters long")

        self.username = username
        self._password = password

        self._followers: Set["User"] = set()
        self._num_of_posts = 0
        self._notifications: List[str] = []

    def follow(self, user: "User") -> None:
        """
        Follow the given user.
        The given user followers set will be updated accordingly.
        Args:
            user: the user to follow

        Returns:
            None
        """
        User.check_if_user_login(self)

        if user == self:
            raise ValueError("Cannot follow yourself")
        user._followers.add(self)
        # print message
        print(f"{self.username} started following {user.username}")

    def unfollow(self, user: "User") -> None:
        """
        Unfollow the given user.
        The given user followers set will be updated accordingly.
        """
        User.check_if_user_login(self)

        if self in user._followers:
            user._followers.remove(self)
            # print message
            print(f"{self.username} unfollowed {user.username}")
        else:
            raise IllegalOperationError("User is not following the given user")

    def publish_post(self, post_type: str, *args) -> Post:
        """
        Publish a post of the given type.
        The post will be created using the post-factory.
        Args:
            post_type: the type of the post to create.
            Will be in ["Text", "Image", "Sale"]
            *args: the arguments to pass to the post.
            See the `PostFactory.py` for more details
        """
        User.check_if_user_login(self)
        new_post = User._post_factory.create_post(post_type, self, *args)
        self._num_of_posts += 1
        return new_post

    def notify(self, message: str, log: bool, extra_message: str = "") -> None:
        """
         Notify the user with the given message.
         If the log is True, the message will be printed to the console
        :param message: the message to notify the user with
        :param log: whether to print the message to the console
        :param extra_message: extra message to print
        :return: None
        """
        self._notifications.append(message)
        if log:
            print(f"notification to {self.username}: {message}{extra_message}")

    def print_notifications(self):
        print(f"{self.username}'s notifications:")
        for n in self._notifications:
            print(n)

    def compare_password(self, password: str) -> bool:
        """
        Compare the given password to the user's password.
        :param password: the password to compare
        :return: True if the given password is the same as the user's password, False otherwise
        """
        return self._password == password

    def get_followers(self) -> Set["User"]:
        """
        Get the user's followers
        :return: the user's followers
        """
        return self._followers

    def __str__(self) -> str:
        return f'User name: {self.username}, Number of posts: {self._num_of_posts}, Number of followers: {len(self._followers)}'

    def __repr__(self) -> str:
        return self.__str__()

    def __eq__(self, other) -> bool:
        if isinstance(other, User):
            return self.username == other.username
        return False

    def __hash__(self) -> int:
        return hash(self.username)

    @staticmethod
    def check_if_user_login(user) -> None:
        from SocialNetwork import SocialNetwork
        # only logged-in users can do this
        if not SocialNetwork.get_instance().is_user_logged_in(user):
            raise NotLoginError("User is not logged in")
