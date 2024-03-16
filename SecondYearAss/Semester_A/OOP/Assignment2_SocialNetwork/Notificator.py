# for type hinting
from __future__ import annotations

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from User import User


# end type hinting

class Notificator:
    """
    A class to notify users.
    This class implements the observer pattern.
    The register method is implemented in the User class, because we notify just the users who are following another user.
    """

    def notify_user(self, user_to_notify: User, notifier: User, message: str, log: bool = False,
                    extra_message: str = ""):
        """
        Notify a user with a message.
        You can also log the message and add an extra message.
        Args:
            user_to_notify: The user to notify
            notifier: The user who is notifying
            message: The message to notify
            log: True if you want to log the message (Default to False).
            extra_message: An extra message to add (Default to an empty string).

        Returns:
            None

        """
        if user_to_notify == notifier:  # Don't notify the user who is notifying
            return

        # user the notify method from the User class
        user_to_notify.notify(message, log, extra_message)

    def notify_all_followers(self, caller: User, message: str, log: bool = False):
        """
        Notify all followers of a user.
        Args:
            caller: The user who is calling the method
            message: The message to notify
            log: True if you want to log the message (Default to False).

        Returns:
            None
        """
        for user in caller.get_followers():  # Notify all followers
            self.notify_user(user, caller, message, log)
